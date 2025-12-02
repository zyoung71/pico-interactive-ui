#pragma once

#include <stack>
#include <pico/time.h>

#include "DisplayInterface.h"
#include "Screen.h"

class ScreenManager
{
private:

    DisplayInterface* display;
    std::stack<Screen*> screens; 
    Screen* selected_screen;

    queue_t control_queue;
    absolute_time_t then = 0;
    
public:
    ScreenManager(DisplayInterface* display);
    ~ScreenManager();

    void PushScreen(Screen* screen);
    void PopScreen();

    void QueueControl(uint32_t action_mask);
    
    void Update();

    friend Screen;
};