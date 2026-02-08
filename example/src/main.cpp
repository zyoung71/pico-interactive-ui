#include <pico/stdio.h>

#include <interactive-ui/ScreenManager.h>
#include <interactive-ui/components/TextComponent.h>

#include <math/Graphics.h>

static constexpr Vec2i32 screen_dimensions = Vec2i32(128, 64);

int main()
{
    stdio_init_all();

    DisplayInterface* display; // The HAL for the actual display screen is implemented by the user externally.
    ScreenManager manager(display);

    Screen menu(&manager, screen_dimensions);

    TextComponent text1(&manager, Vec2i32{20, 20}, "Hello", nullptr, 1, &menu);

    TextBoxComponent text2(&manager, Vec2i32{0, 0}, Vec2i32{31, 15}, Vec2i32{2, 2}, "TEXT", nullptr, 1, &menu);
    
    text1.AddComponentTable(&menu, nullptr, nullptr, &text2, nullptr);
    text2.AddComponentTable(&menu, nullptr, nullptr, nullptr, &text1);

    manager.PushScreen(&menu);

    while (1)
    {
        manager.Update();
    }

    return 0;
}