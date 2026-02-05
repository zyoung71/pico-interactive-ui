#pragma once

#include <stack>
#include <unordered_set>
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
    std::unordered_set<Screen*> screen_set; // stacks cannot be traversed, so there is this. it keeps a record of all screens that have been added
    Screen* selected_screen;

    queue_t control_queue;
    float last_dt = 0.f;

    int master_component_moving_reference_count = 0;

    float refresh_rate = 0.f; // if zero, runs as fast as possible, still allowing manual updating only
    float refresh_period;

public:
    ScreenManager(DisplayInterface* const display);
    ~ScreenManager();

    inline DisplayInterface* GetDisplay() const
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
    inline float GetRefreshRate() const
    {
        return refresh_rate;
    }
    inline void SetRefreshRate(float hz)
    {
        refresh_rate = hz;
        if (hz > 0.f)
            refresh_period = 1.f / hz;
        else
            refresh_period = 0.f;
    }

    void QueueControl(uint32_t action_mask);
    
    void UpdateDeltaTime();
    void Update();
    void UpdateIfAnyComponentMoving();

    friend Screen;
    friend Component;
};