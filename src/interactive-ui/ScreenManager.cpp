#include <interactive-ui/ScreenManager.h>
#include <interactive-ui/Screen.h>
#include <interactive-ui/Component.h>

ScreenManager::ScreenManager(DisplayInterface* const display)
    : display(display), selected_screen(nullptr)
{
    queue_init(&control_queue, sizeof(uint32_t), 4);
}

ScreenManager::~ScreenManager()
{
    queue_free(&control_queue);
}

void ScreenManager::PushScreen(Screen* screen)
{
    screens.push(screen);
    screen_set.insert(screen);
    screen->manager = this; // Should be set already.
    if (selected_screen)
        selected_screen->OnScreenDeselect();
    selected_screen = screen;
    selected_screen->OnScreenSelect();
    Update();
}

void ScreenManager::PopScreen()
{
    screens.pop();
    selected_screen->OnScreenDeselect();
    selected_screen = screens.top();
    selected_screen->OnScreenSelect();
    Update();
}

void ScreenManager::QueueControl(uint32_t action_mask)
{
    queue_try_add(&control_queue, &action_mask);
}

float ScreenManager::UpdateDeltaTime()
{
    static absolute_time_t then = 0;
    absolute_time_t now = to_us_since_boot(get_absolute_time());
    absolute_time_t dt_us = absolute_time_diff_us(then, now);
    last_dt = dt_us * 1e-6f;
    then = now;
    return last_dt;
}

void ScreenManager::Update()
{
    display->ClearDisplay();
    selected_screen->Update(UpdateDeltaTime());
    display->UpdateDisplay();
}

void ScreenManager::UpdateIfAnyComponentMoving()
{
    // at least one component globally is moving.
    // this function is intended to be called in the main loop
    // therefore it is ideal we guard the set search
    if (master_component_moving_reference_count > 0)
    {
        UpdateDeltaTime();
        for (auto s : screen_set)
        {
            if (s == screens.top())
            {
                Update();
                continue;
            }
            if (s->HasMovingComponent())
                s->Update(last_dt);
        }        
    }
}