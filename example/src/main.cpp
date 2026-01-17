#include <pico/stdio.h>

#include <interactive-ui/ScreenManager.h>
#include <ClockComponent.h>

#include <math/Graphics.h>

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

    TextBoxComponent text(&manager, Vec2u32{0, 0}, Vec2u32{31, 15}, Vec2u32{2, 2}, "TEXT", nullptr, 1);
    
    text.AddComponentTable(&menu, nullptr, nullptr, nullptr, &clock_component);
    clock_component.AddComponentTable(&menu, nullptr, nullptr, &text, nullptr);

    int id = clock_component.AddAction([](const Event* ev, void* ptr){
        // When the clock component is selected with any select button or etc.
        RealTimeClock* clock = (RealTimeClock*)ptr;
        clock->Use24HourTime(); // Changes to 24-hour time.
    }, &clock);

    int id2 = text.AddAction([](const Event* ev, void* ptr){
        // When the text box is selected with a button or etc.
        ClockComponent* clk = (ClockComponent*)ptr;
        MovementAnimation animation(clk, graphics::easing::lut_cubic_in_out);
        animation.duration = 1.f;
        animation.end_pos = Vec2u32{50, 40};
        clk->Move(&animation);
    }, &clock_component);

    manager.PushScreen(&menu);

    while (1)
    {
        manager.Update();
    }

    return 0;
}