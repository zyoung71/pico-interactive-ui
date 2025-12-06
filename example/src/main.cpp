#include <pico/stdio.h>

#include <interactive-ui/ScreenManager.h>
#include <ClockComponent.h>

static constexpr Vec2u32 screen_dimensions = Vec2u32(128, 64);

// Example: Media player
int main()
{
    stdio_init_all();

    DisplayInterface* display; // The HAL for the actual display screen is implemented by the user externally.
    ScreenManager manager(display);

    Screen play_menu(&manager, screen_dimensions);
    Screen queue_menu(&manager, screen_dimensions);
    Screen settings_menu(&manager, screen_dimensions);

    // The settings menu may show the time.
    RealTimeClock clock(4, 5, 6, i2c0);
    ClockComponent clock_component(&settings_menu, &clock, 0);
    settings_menu.AddComponent(&clock_component);

    manager.PushScreen(&play_menu);

    while (1)
    {
        manager.Update();
    }

    return 0;
}