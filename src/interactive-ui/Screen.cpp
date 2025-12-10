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
    : dimensions(dimensions), data(manager->display, manager, this), hovered_component(nullptr)
{
}

void Screen::AddComponent(Component* component)
{
    component->data.screen = this; // Components may be used across many screens, and ones that do should not use per-screen algorithms.
    components.push_back(component);
}

void Screen::SortComponents()
{
    // Find the first selectable component and hover it if there is no hover already.
    if (!hovered_component)
    {
        for (auto&& c : components)
        {
            if (c->selectable)
            {
                hovered_component = (SelectableComponent*)c; // A little dangerous, but only if you are dumb enough to make it dangerous.
                SelectableComponent** neighbors = hovered_component->neighboring_components;
                if (neighbors[0] || neighbors[1] || neighbors[2] || neighbors[3]) // Only hover if any neighboring components are set which defines it as something intended to be hovered.
                {
                    hovered_component->OnComponentHovered();
                    break;
                }
            }
        }
    }
    std::sort(components.begin(), components.end(), _ComponentCompare);
}

void Screen::OnControl(uint32_t control_mask) // Trying to keep this function modular.
{
    NavigateToComponent(control_mask);
    ActOnComponent(control_mask);
}

void Screen::OnScreenSelect()
{
    // Note: could be needed later but not certain. Will get back to this when necessary.
    //if (hovered_component)
    //    hovered_component->OnComponentHovered();
    for (auto&& c : components)
    {
        c->ForceVisibility(true);
    }
}

void Screen::OnScreenDeselect()
{
    //if (hovered_component)
    //    hovered_component->OnComponentUnhovered();
    for (auto&& c : components)
    {
        c->ForceVisibility(false);
    }
}

void Screen::Update(float dt)
{   
    uint32_t control_mask;
    if (queue_try_remove(&data.manager->control_queue, &control_mask))
    {
        OnControl(control_mask);
    }

    for (auto&& c : components) // Draw components last.
    {
        c->Update(dt);
        hovered_component->DrawHover();
    }
}

bool Screen::NavigateToComponent(uint32_t control_mask)
{
    bool success = false;

    // This approach allows for multi-input like moving up and to the right on the same update.
    if (control_mask & DIRECTIONAL_UP)
    {
        SelectableComponent* opt = hovered_component->up_component;
        if (opt != nullptr)
        {
            hovered_component->OnComponentUnhovered();
            hovered_component = opt;
            success = true;
        }
    }
    if (control_mask & DIRECTIONAL_RIGHT)
    {
        SelectableComponent* opt = hovered_component->right_component;
        if (opt != nullptr)
        {
            hovered_component->OnComponentUnhovered();
            hovered_component = opt;
            success = true;
        }
    }
    if (control_mask & DIRECTIONAL_DOWN)
    {
        SelectableComponent* opt = hovered_component->down_component;
        if (opt != nullptr)
        {
            hovered_component->OnComponentUnhovered();
            hovered_component = opt;
            success = true;
        }
    }
    if (control_mask & DIRECTIONAL_LEFT)
    {
        SelectableComponent* opt = hovered_component->left_component;
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
    if (control_mask & SELECT0)
    {
        hovered_component->Select0();
    }
    if (control_mask & SELECT1)
    {
        hovered_component->Select1();
    }
    if (control_mask & BACK)
    {
        // Backs out the menu if there's one pushed over the main screen.
        if (data.manager->screens.size() > 1)
        {
            data.manager->PopScreen();
        }
    }
}