#pragma once

#include <stack>
#include <pico/time.h>
#include <pico/util/queue.h>

#include "DisplayInterface.h"

class Screen;

class ScreenManager
{
private:

    DisplayInterface* const display;
    std::stack<Screen*> screens; 
    Screen* selected_screen;

    queue_t control_queue;
    absolute_time_t then = 0;
    
public:
    ScreenManager(DisplayInterface* const display);
    ~ScreenManager();

    inline DisplayInterface* const GetDisplay() const
    {
        return display;
    }

    void PushScreen(Screen* screen);
    void PopScreen();

    inline size_t GetScreenCount() const
    {
        return screens.size();
    }

    void QueueControl(uint32_t action_mask);
    
    void Update();
    void UpdateIfAnyComponentMoving();

    friend Screen;
};