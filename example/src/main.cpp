#include <pico/stdio.h>

#include <interactive-ui/ScreenManager.h>
#include <ClockComponent.h>

static constexpr Vec2u32 screen_dimensions = Vec2u32(128, 64);

int main()
{
    stdio_init_all();

    DisplayInterface* display; // The HAL for the actual display screen is implemented by the user externally.
    ScreenManager manager(display);

    Screen menu(&manager, screen_dimensions);

    RealTimeClock clock(4, 5, 6, i2c0);
    ClockComponent clock_component(&manager, &clock, 0, &menu);
    menu.AddComponent(&clock_component);

    int id = clock_component.AddAction([](const Event* ev, void* ptr){
        // When the clock component is selected with any select button or etc.
        RealTimeClock* clock = (RealTimeClock*)ptr;
        clock->Use24HourTime(); // Changes to 24-hour time.
    }, &clock);

    manager.PushScreen(&menu);

    while (1)
    {
        manager.Update();
    }

    return 0;
}