#pragma once

#include <stack>
#include <pico/time.h>
#include <pico/util/queue.h>

#include "DisplayInterface.h"

class Screen;
class Component;

class ScreenManager
{
private:
    DisplayInterface* const display;
    std::stack<Screen*> screens; 
    Screen* selected_screen;

    queue_t control_queue;
    float last_dt = 0.f;
    
    mutable int component_moving_reference_count = 0;

public:
    ScreenManager(DisplayInterface* const display);
    ~ScreenManager();

    inline DisplayInterface* const GetDisplay() const
    {
        return display;
    }

    void PushScreen(Screen* screen);
    void PopScreen();

    inline Screen* GetCurrentScreen() const
    {
        return selected_screen;
    }

    inline size_t GetScreenCount() const
    {
        return screens.size();
    }

    void QueueControl(uint32_t action_mask);
    
    void UpdateDeltaTime();
    void Update();
    void UpdateIfAnyComponentMoving();

    friend Screen;
    friend Component;
};