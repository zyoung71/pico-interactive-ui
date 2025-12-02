#include <interactive-ui/ScreenManager.h>

ScreenManager::ScreenManager(DisplayInterface* display)
    : display(display)
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
    screen->data.manager = this; // Should be set already.
    selected_screen = screen;
    selected_screen->OnScreenSelect();
}

void ScreenManager::PopScreen()
{
    screens.pop();
    selected_screen->OnScreenDeselect();
    selected_screen = screens.top();
}

void ScreenManager::QueueControl(uint32_t action_mask)
{
    queue_try_add(&control_queue, &action_mask);
}

void ScreenManager::Update()
{
    absolute_time_t dt_us = absolute_time_diff_us(then, get_absolute_time());
    selected_screen->Update(dt_us * 1e-6f);
}