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
    : dimensions(dimensions), data(manager->display, manager, this), selected_widget(nullptr)
{
}

void Screen::AddComponent(Component* component)
{
    components.push_back(component);
}

void Screen::SortComponents()
{
    // Find the first selectable component and select it if there is no selection.
    if (!selected_widget)
    {
        for (auto&& c : components)
        {
            if (c->selectable)
            {
                selected_widget = (SelectableComponent*)c; // A little dangerous, but only if you are dumb enough to make it dangerous.
                selected_widget->is_selected = true;
                selected_widget->OnComponentSelected();
                break;
            }
        }
    }
    std::sort(components.begin(), components.end(), _ComponentCompare);
}

void Screen::OnControl(uint32_t control_mask)
{
    NavigateToComponent(control_mask);
}

void Screen::OnScreenSelect()
{
    for (auto&& c : components)
    {
        c->SetVisible(true);
    }
}

void Screen::OnScreenDeselect()
{
    for (auto&& c : components)
    {
        c->SetVisible(false);
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
    }
}

bool Screen::NavigateToComponent(uint32_t control_mask)
{
    bool success = false;

    // This approach allows for multi-input like moving up and to the right.
    if (control_mask & DIRECTIONAL_UP)
    {
        SelectableComponent* opt = selected_widget->up_component;
        if (opt != nullptr)
        {
            selected_widget->is_selected = false;
            selected_widget->OnComponentDeselected();
            selected_widget = opt;
            success = true;
        }
    }
    if (control_mask & DIRECTIONAL_RIGHT)
    {
        SelectableComponent* opt = selected_widget->right_component;
        if (opt != nullptr)
        {
            selected_widget->is_selected = false;
            selected_widget->OnComponentDeselected();
            selected_widget = opt;
            success = true;
        }
    }
    if (control_mask & DIRECTIONAL_DOWN)
    {
        SelectableComponent* opt = selected_widget->down_component;
        if (opt != nullptr)
        {
            selected_widget->is_selected = false;
            selected_widget->OnComponentDeselected();
            selected_widget = opt;
            success = true;
        }
    }
    if (control_mask & DIRECTIONAL_LEFT)
    {
        SelectableComponent* opt = selected_widget->left_component;
        if (opt != nullptr)
        {
            selected_widget->is_selected = false;
            selected_widget->OnComponentDeselected();
            selected_widget = opt;
            success = true;
        }
    }
    if (success)
    {
        selected_widget->is_selected = true;
        selected_widget->OnComponentSelected();
    }

    return success;
}