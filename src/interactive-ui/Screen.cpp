#include <interactive-ui/Screen.h>

#include <interactive-ui/SelectableComponent.h>
#include <interactive-ui/ScreenManager.h>

Screen::Screen(ScreenManager* manager, uint32_t width, uint32_t height)
    : Screen(manager, Vec2u32(width, height))
{
}

Screen::Screen(ScreenManager* manager, const Vec2u32& dimensions)
    : dimensions(dimensions), data(manager->display, manager, this)
{
}

void Screen::AddComponent(Component* component)
{
    components.emplace(component);
}

void Screen::RemoveComponent(Component* component)
{
    components.erase(component);
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
    for (auto&& c : components)
    {
        c->Update(dt);
        c->Draw();
    }
    
    uint32_t control_mask;
    if (this == data.manager->selected_screen)
    {
        if (queue_try_remove(&data.manager->control_queue, &control_mask))
        {
            OnControl(control_mask);
        }
    }   
}

bool Screen::NavigateToComponent(uint32_t control_mask)
{
    bool success = false;

    // This approach allows for multi-input like moving up and to the right.
    if (control_mask & DIRECTIONAL_UP)
    {
        ResultOptional opt = selected_widget->GetUpComponent();
        selected_widget = opt;
        success = opt.supporting;
    }
    if (control_mask & DIRECTIONAL_RIGHT)
    {
        ResultOptional opt = selected_widget->GetRightComponent();
        selected_widget = opt;
        if (!success)
            success = opt.supporting;
    }
    if (control_mask & DIRECTIONAL_DOWN)
    {
        ResultOptional opt = selected_widget->GetDownComponent();
        selected_widget = opt;
        if (!success)
            success = opt.supporting;
    }
    if (control_mask & DIRECTIONAL_LEFT)
    {
        ResultOptional opt = selected_widget->GetLeftComponent();
        selected_widget = opt;
        if (!success)
            success = opt.supporting;
    }
    selected_widget->SetSelected(true);

    return success;
}