#include <interactive-ui/Screen.h>
#include <interactive-ui/SelectableComponent.h>
#include <interactive-ui/ScreenManager.h>

#include <algorithm>

bool Screen::_ComponentCompare(const Component* a, const Component* b)
{
    return a->GetZLayer() < b->GetZLayer();
}

Screen::Screen(ScreenManager* manager, uint32_t width, uint32_t height)
    : Screen(manager, Vec2u32(width, height))
{
}

Screen::Screen(ScreenManager* manager, const Vec2u32& dimensions)
    : dimensions(dimensions), manager(manager), display(manager->display), hovered_component(nullptr)
{
}

void Screen::AddComponent(Component* component)
{
    components.push_back(component);
    component_set.insert(component);
}

void Screen::RemoveComponent(Component* comp)
{
    if (component_set.erase(comp))
    {
        std::erase(components, comp);
        comp->screen_set.erase(this);
    }
}

void Screen::HoverComponent(SelectableComponent* comp)
{
    if (component_set.contains((Component*)comp))
    {
        if (hovered_component)
                hovered_component->OnComponentUnhovered();

        hovered_component = comp; // casting is fine here
        hovered_component->OnComponentHovered();
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
                        return;
                    }
                }
            }
        }
    }
}

void Screen::SortComponents()
{
    std::sort(components.begin(), components.end(), _ComponentCompare);
    HoverDefaultComponent();
}

void Screen::SetComponentZLayer(Component* comp, int32_t z_layer)
{
    if (component_set.contains(comp))
    {
        comp->z_layer = z_layer;
    }
}

void Screen::OnControl(uint32_t control_mask) // Trying to keep this function modular.
{
    NavigateToComponent(control_mask);
    ActOnComponent(control_mask);
}

void Screen::OnScreenSelect()
{
    if (hovered_component)
    {
        hovered_component->allow_hover_draw = true;
        //hovered_component->OnComponentHovered();
    }
    for (auto&& c : components)
    {
        c->ForceVisibility(true);
    }
}

void Screen::OnScreenDeselect()
{
    if (hovered_component)
    {
        hovered_component->allow_hover_draw = false;
        //hovered_component->OnComponentUnhovered();
    }
    for (auto&& c : components)
    {
        c->ForceVisibility(false);
    }
}

void Screen::Update(float dt)
{   
    uint32_t control_mask;
    while (queue_try_remove(&manager->control_queue, &control_mask))
    {
        OnControl(control_mask);
    }

    for (auto&& c : components) // Draw components last.
    {
        c->Update(dt);
    }
    
    if (hovered_component)
    {
        if (hovered_component->allow_hover_draw)
            hovered_component->DrawHover();
    }
}

bool Screen::NavigateToComponent(uint32_t control_mask)
{
    if (!hovered_component)
        return false;

    bool success = false;

    // This approach allows for multi-input like moving up and to the right on the same update.
    if (control_mask & DIRECTIONAL_UP)
    {
        SelectionTable& table = hovered_component->component_lut[this];
        SelectableComponent* opt = table.up_component;
        if (opt != nullptr)
        {
            hovered_component->OnComponentUnhovered();
            hovered_component = opt;
            success = true;
        }
    }
    if (control_mask & DIRECTIONAL_RIGHT)
    {
        SelectionTable& table = hovered_component->component_lut[this];
        SelectableComponent* opt = table.right_component;
        if (opt != nullptr)
        {
            hovered_component->OnComponentUnhovered();
            hovered_component = opt;
            success = true;
        }
    }
    if (control_mask & DIRECTIONAL_DOWN)
    {
        SelectionTable& table = hovered_component->component_lut[this];
        SelectableComponent* opt = table.down_component;
        if (opt != nullptr)
        {
            hovered_component->OnComponentUnhovered();
            hovered_component = opt;
            success = true;
        }
    }
    if (control_mask & DIRECTIONAL_LEFT)
    {
        SelectionTable& table = hovered_component->component_lut[this];
        SelectableComponent* opt = table.left_component;
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
    }

    return success;
}

void Screen::ActOnComponent(uint32_t control_mask)
{
    if (hovered_component)
    {
        for (decltype(control_mask) i = 0; i < sizeof(control_mask) * 8; i++)
        {
            decltype(control_mask) binrep = 1 << i;
            if (control_mask & binrep)
                hovered_component->Control((ControlAction)binrep);
        }

        if (hovered_component->cancel_master_back_action)
            return;
    }
    
    // Does not require an active hovered component.
    if (control_mask & BACK)
    {
        // Backs out the menu if there's one pushed over the main screen.
        if (manager->screens.size() > 1)
        {
            manager->PopScreen();
        }
    }
}