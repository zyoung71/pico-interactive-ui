#include <interactive-ui/Screen.h>
#include <interactive-ui/components/PaddingComponent.h>
#include <interactive-ui/SelectableComponent.h>
#include <interactive-ui/ScreenManager.h>

#include <algorithm>

class DefaultHoverDesignComponent : public PaddingComponent
{
public:
    DefaultHoverDesignComponent(ScreenManager* manager, Screen* initial_screen)
        : PaddingComponent(manager, Vec2i32{-1, -1}, Vec2i32{0, 0}, Screen::hover_z_layer, initial_screen) {}

    void Update(float dt, const Screen* screen) override
    {
        screen->HoverChange(true);
        PaddingComponent::Update(dt, screen);
    }
};

bool Screen::_ComponentCompare(const Component* a, const Component* b)
{
    return a->z_layer < b->z_layer;
}

void Screen::HoverChange(bool instant) const
{
    constexpr Vec2i32 hover_outline_width = Vec2i32{2, 2};

    if (instant || animation_hover_duration <= 0.f)
    {
        hover_design->GetComponent()->origin_position = hovered_component->origin_position;
        hover_design->GetComponent()->draw_dimensions = hovered_component->draw_dimensions;
        hover_design->GetComponent()->draw_dimensions.min -= hover_outline_width;
        hover_design->GetComponent()->draw_dimensions.max += hover_outline_width;
        return;
    }
    MovementAnimation move(hover_design->GetComponent(), *easing_func);
    move.duration = animation_hover_duration;
    move.end_pos = hovered_component->origin_position;
    move.end_scale = hovered_component->draw_dimensions;
    move.end_scale.min -= hover_outline_width;
    move.end_scale.max += hover_outline_width;
    move.transpose = true;
    move.scale = true;
    hover_design->GetComponent()->Move(move);
}

Screen::Screen(ScreenManager* manager, uint32_t width, uint32_t height)
    : Screen(manager, Vec2u32(width, height))
{
}

Screen::Screen(ScreenManager* manager, const Vec2u32& dimensions)
    : dimensions(Vec2i32{0, 0}, (Vec2i32)dimensions), manager(manager), display(manager->display), hovered_component(nullptr),
    hover_design(std::make_unique<DefaultHoverDesignComponent>(manager, this))
{
}

Screen::Screen(const Screen& to_copy)
    : dimensions(to_copy.dimensions), components(to_copy.components), component_set(to_copy.component_set),
    component_moving_reference_count(0), hovered_component(to_copy.hovered_component), animation_hover_duration(to_copy.animation_hover_duration),
    display(to_copy.display), manager(to_copy.manager), hover_design(std::make_unique<DefaultHoverDesignComponent>(to_copy.manager, this))
{
}

void Screen::AddComponent(Component* component)
{
    if (component_set.insert(component).second)
    {
        components.push_back(component);
        component->screen_set.insert(this);
    }
}

void Screen::RemoveComponent(Component* comp)
{
    if (component_set.erase(comp))
    {
        std::erase(components, comp);
        comp->screen_set.erase(this);
    }
}

void Screen::HoverComponent(SelectableComponent* comp, bool instant) // may be used to force hovers on things that have no neighbors
{
    if (component_set.contains((Component*)comp))
    {
        if (hovered_component)
                hovered_component->OnComponentUnhovered();

        hovered_component = comp;
        hovered_component->OnComponentHovered();
        // in order for any component to move automatically,
        // the screen manager must have the screen added to its set
        manager->screen_set.insert(this);
        HoverChange(instant);
    }
}

void Screen::UnhoverComponent()
{
    if (hovered_component)
    {
        hovered_component->OnComponentUnhovered();
        hovered_component = nullptr;
    }
}

void Screen::HoverDefaultComponent()
{
    // Find the first selectable component and hover it if there is no hover already.
    if (!hovered_component)
    {
        for (auto it = components.rbegin(); it != components.rend(); ++it) // We should find something with the highest Z number.
        {
            Component* c = *it;
            if (c->selectable)
            {
                SelectableComponent* sc = (SelectableComponent*)c; // A little dangerous, but only if you are dumb enough to make it dangerous.
                if (sc->component_lut.contains(this))
                {
                    SelectableComponent** neighbors = sc->component_lut[this].neighboring_components;
                    if (neighbors[0] || neighbors[1] || neighbors[2] || neighbors[3]) // Only hover if any neighboring components are set which defines it as something intended to be hovered.
                    {
                        hovered_component = sc;
                        hovered_component->OnComponentHovered();
                        HoverChange(true);
                        return;
                    }
                }
            }
        }
    }
}

bool Screen::IsComponentHoverable(const SelectableComponent* comp) const
{
    if (!comp->component_lut.contains(this))
        return false;

    const SelectableComponent* const* neighbors = comp->component_lut.at(this).neighboring_components;
    return neighbors[0] || neighbors[1] || neighbors[2] || neighbors[3];
}

void Screen::SortComponents()
{
    std::sort(components.begin(), components.end(), _ComponentCompare);
    HoverDefaultComponent();
}

void Screen::OnControl(uint64_t control_mask) // Trying to keep this function modular.
{
    NavigateToComponent(control_mask);
    ActOnComponent(control_mask);
}

void Screen::OnScreenSelect()
{
    if (hovered_component)
    {
        hover_design->GetComponent()->ForceVisibility(true);
        //hovered_component->OnComponentHovered(); // not used, but could be later
    }
    for (auto c : components)
    {
        c->ForceVisibility(true);
        c->OnEnterScreen(this);
    }
}

void Screen::OnScreenDeselect()
{

    if (hovered_component)
    {
        hover_design->GetComponent()->ForceVisibility(false);
        hovered_component->locked = false;
        //hovered_component->OnComponentUnhovered(); // not used, but could be later
    }
    for (auto c : components)
    {
        c->ForceVisibility(false);
        c->OnExitScreen(this);
    }
    hover_design->SetThickness(1);
}

void Screen::ProcessQueuedControls()
{
    uint64_t control_mask;
    while (queue_try_remove(&manager->control_queue, &control_mask))
    {
        OnControl(control_mask);
    }
}

void Screen::Update(float dt)
{   
    for (auto c : components)
    {
        c->Update(dt, this);
    }
}

bool Screen::NavigateToComponent(uint64_t control_mask)
{
    if (!hovered_component || manager->enable_cursor)
        return false;

    if (hovered_component->locked)
        return false;

    bool success = false;

    // This approach allows for multi-input like moving up and to the right on the same update.
    SelectionTable& table = hovered_component->component_lut[this];
    SelectableComponent* opt;
    if (control_mask & DIRECTIONAL_UP)
    {
        opt = table.up_component;
        if (opt != nullptr)
        {
            hovered_component->OnComponentUnhovered();
            hovered_component = opt;
            success = true;
        }
    }
    if (control_mask & DIRECTIONAL_RIGHT)
    {
        opt = table.right_component;
        if (opt != nullptr)
        {
            hovered_component->OnComponentUnhovered();
            hovered_component = opt;
            success = true;
        }
    }
    if (control_mask & DIRECTIONAL_DOWN)
    {
        opt = table.down_component;
        if (opt != nullptr)
        {
            hovered_component->OnComponentUnhovered();
            hovered_component = opt;
            success = true;
        }
    }
    if (control_mask & DIRECTIONAL_LEFT)
    {
        opt = table.left_component;
        if (opt != nullptr)
        {
            hovered_component->OnComponentUnhovered();
            hovered_component = opt;
            success = true;
        }
    }
    if (success)
    {
        hovered_component->OnComponentHovered();
        HoverChange();
    }

    return success;
}

void Screen::ActOnComponent(uint64_t control_mask)
{
    if (hovered_component)
    {
        constexpr decltype(control_mask) bits = sizeof(control_mask) * 8;
        for (decltype(control_mask) i = 0; i < bits; i++)
        {
            decltype(control_mask) binrep = 1 << i;
            if ((control_mask & binrep) && (hovered_component->control_enables & binrep))
                hovered_component->Control((ControlAction)binrep);
        }
    }
    
    // Does not require an active hovered component.
    if (control_mask & BACK)
    {
        if (hovered_component->locked)
        {
            hovered_component->Lock(false);
            return;
        }
        
        // Backs out the menu if there's one pushed over the main screen.
        if ((hovered_component->control_enables & ControlAction::BACK) && (manager->screens.size() > 1))
        {
            manager->PopScreen();
        }
    }
}