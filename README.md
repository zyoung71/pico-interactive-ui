# Raspberry Pi Pico Interactive UI Library
#### Table of contents:
1. Setup
2. Implement your screen
3. Simple text display
4. Component selection
5. Multiple screen usage
6. Animations
7. Custom component creation

## 1. Setup
Use this library as a submodule in your project that .<br>
`git submodule add https://github.com/zyoung71/pico-interactive-ui lib/pico-interactive-ui`<br>

Edit your CMake file properly to include the library.<br>
```cmake
add_subdirectory(lib/pico-interactive-ui)

target_link_libraries(${CMAKE_PROJECT_NAME}
    pico-interactive-ui
)
```

Be sure to initialize and update submodules. No recursiveness needs to be applied here.<br>
`git submodule update --init --remote`<br>

## 2. Implement your screen
The abstract class `DisplayInterface` requires to be extended from to implement screen behavior. Include the header and create your class like so. Note that this example contains the bare minimum, you will need to implement your own ways of object construction.
```c++
#include <interactive-ui/DisplayInterface.h>

class MyDisplay : public DisplayInterface
{
public:

    Vec2u32 GetDimensions() const override;

    void UpdateDisplay() override;
    void Power(bool power_on) override;
    void ClearDisplay() override;
    void InvertColors() override;
    void SetBrightness(uint8_t brightness) override;

    void DrawPixel(Vec2i32 pos, RGBA color) override;
    void DrawPixel(int32_t x, int32_t y, RGBA color) override;
};
```
The `DrawPixel` function will likely perform the same action. To see a functioning example of implemented screen hardware, see https://github.com/zyoung71/pico-ssd1306-ui.

## 3. Simple text display
To begin with displaying things, construct your screen object, a `ScreenManager`, and a `Screen` object first like so:
```c++
#include <interactive-ui/Screen.h>

MyDisplay display; // your display
ScreenManager manager = ScreenManager(&display); // the ScreenManager holds all screens as a stack
Screen screen = Screen(&manager, display.GetDimensions()); // each screen holds a set of Component pointers
```
Initialize the following inside your `main()` or any other function that runs before the main loop. Also include how the screen updates.
```c++
int main()
{
    display.Power(true); // verifies power is on
    display.ClearDisplay(); // clears buffer

    manager.EnableCBF(true); // optional: click between frames
    manager.SetTargetRefreshRate(30.f); // optional: refresh rate

    screen.SortComponents(); // sort components by Z-layer
    manager.PushScreen(&screen); // set first screen
    manager.ForceUpdate(); // use this where necessary if not using a refresh rate

    while (1)
    {
        // if using a refresh rate, call this
        manager.Update();
    }
}
```
Components are the way to display different things on your screen. Each component extends from the `Component` class. For text display, a component exists already and can be created like so. Note that components can be created as static, global memory to work easier with callbacks.

```c++
#include <interactive-ui/components/TextComponent.h>

// constructs a TextComponent located in the center
// of the screen (50% x, 50% y) at Z-layer 0 found
// on the screen you constructed
TextComponent text = TextComponent(&manager, Vec2f{0.5f, 0.5f}, "Hello!", &fonts::default_font, 0, &screen);

...() // some init function, like main()
{
    // all optional settings below:

    // sets the alignment to make the text perfectly centered,
    // instead of drawing down and to the left from the origin point
    text.SetAlignment(AlignmentHorizontal::CENTER, AlignmentVertical::CENTER);
    
    // triples the size of the font (will 9x in total area, because 3^2 = 9)
    text.SetFontScale(3);
}
```
Because we passed the memory address of the screen into the `TextComponent` constructor, it is automatically added to the screen.

## 4. Component selection
This library supports what's known as component selection, where certain components can be "hovered" and selected with different controls, like a left or right click behavior. All components capable of this are extended from the `SelectableComponent` class. For example, `TextComponent` and `BitmapComponent` are both child classes of `SelectableComponent`. When more than one `SelectableComponent` object is created, you can add a directional table to each component, telling it what component it should map to next when any directional controls are input. Here is an example using another `TextComponent`.

```c++
TextComponent hello = TextComponent(&manager, Vec2f{0.5f, 0.33f}, "Hello", &fonts::default_font, 0, &screen);

TextComponent world = TextComponent(&manager, Vec2f{0.5f, 0.66f}, "World!", &fonts::default_font, 0, &screen);

...() // some init function, like main()
{
    hello.SetAlignment(AlignmentHorizontal::CENTER, AlignmentVertical::CENTER);
    world.SetAlignment(AlignmentHorizontal::CENTER, AlignmentVertical::CENTER);

    // ordered by UP, DOWN, LEFT, RIGHT
    // the "Hello" text will point down to "World!"
    hello.AddComponentTable(&screen, nullptr, &world, nullptr, nullptr);
    // the "World!" text will point up to "Hello"
    world.AddComponentTable(&screen, &hello, nullptr, nullptr, nullptr);

    // by default, hover the "Hello" text first.
    // the boolean value is if the hover animation
    // is instant or not. because this is the first
    // thing to show upon program start, it makes
    // sense to have no animation
    screen.HoverComponent(&hello, true);
}
```
Now the two components point to each other and can go back and forth. To queue the controls, use this.
```c++
manager.QueueControl(ControlAction::DIRECTIONAL_UP);
```
or
```c++
manager.QueueControl(ControlAction::DIRECTIONAL_DOWN);
```
These can also be combined by treating it as a bitmask. If you want to move up and to the right in the same sequence, use this.
```c++
manager.QueueControl(DIRECTIONAL_UP | DIRECTIONAL_RIGHT);
```
By default, the component that is currently hovered will draw a box around its draw dimensions. All included components support automatic draw dimension calculation and updating. If creating your own component, you may have to manage that yourself.

To add callbacks based on what is pressed, use the `AddAction` function.
```c++
// if CBF is enabled, this is run immediately
// instead of waiting for the next update (frame)
int callback_id = hello.AddAction([](const Event* ev, void* ptr){
    // will be called every time any control
    // is acted upon the component.
    // NOTE: this ALSO includes directional
    // controls! if you only want to detect
    // something like a selection, use as follows:
    ComponentSelectEvent* event = static_cast<ComponentSelectEvent*>(ev);

    // this only returns one control action.
    // if multiple were used, this callback
    // function is called as many times as
    // control actions queued
    if (event->GetContol() == ControlAction::SELECT0)
    {
        // your code
    }
});
```

## 5. Multiple screen usage
Because the `ScreenManager` class contains a stack of screens (using `std::stack`) multiple screens can be used and treated like a window system. Already, we used `manager.PushScreen(&screen);` to add the first screen, and you do the same for others. Here's an example for a button that when selected, increments a counter shown on a different screen using a second button below it.

```c++
#include <interactive-ui/Screen.h>
#include <interactive-ui/components/TextComponent.h>

MyDisplay display;
ScreenManager manager(&display);
Screen main_screen(&manager, display.GetDimensions());
Screen counter_screen(&manager, display.GetDimensions());

TextBoxComponent button_counter(&manager, Vec2f{0.5f, 0.33f}, Vec2i32{20, 20}, "Click Me", &fonts::default_font, 0, &main_screen);

TextBoxComponent button_goto(&manager, Vec2f{0.5, 0.66f}, Veci32{20, 15}, 0, &main_screen);

char count_buff[32];
uint32_t count = 0;
TextComponent counter(&manager, Vec2i32{0.5f, 0.5f}, count_buff, &fonts::default_font, 0, &counter_screen);

int main()
{
    display.Power(true);
    display.ClearDisplay();

    manager.EnableCBF(true);
    manager.SetTargetRefreshRate(20.f);

    button_counter.AddComponentTable(&main_screen, nullptr, &button_goto, nullptr, nullptr);
    button_goto.AddComponentTable(&main_screen, &button_counter, nullptr, nullptr, nullptr);

    // the text on the other screen does not need
    // a component table as it does not map
    // to any other component on its screen
    // (as it is the only component on its screen)

    main_screen.HoverComponent(&button_counter, true);

    // counter button action --- every select, counter increments
    std::ignore = button_counter.AddAction([](const Event* ev, void*){
        ComponentSelectEvent* event = static_cast<ComponentSelectEvent*>(ev);

        if (event->GetControl() == SELECT0)
        {
            count++;
            snprintf(count_buff, sizeof(count_buff), "Count: %u", count); // update text buffer
        }
    });
    // goto button action --- every select, goes to the counter screen
    std::ignore = button_goto.AddAction([](const Event* ev, void*){
        ComponentSelectEvent* event = static_cast<ComponentSelectEvent*>(ev);

        if (event->GetControl() == SELECT0)
        {
            manager.PushScreen(&counter_screen);
        }
    });

    main_screen.SortComponents();
    counter_screen.SortComponents();
    manager.PushScreen(&main_screen);

    // your control inputs are expected to be
    // implemented somehow here

    while (1)
    {
        manager.Update();
    }
}
```

By default, the `ControlAction::BACK` control automatically pops the screen off the stack if the stack size is greater than one. If you want to pop the screen off the stack in another manner, either use `manager.QueueControl(BACK)`, or more effectively and more efficient, `manager.PopScreen()`.

## 6. Animations
WIP --- the rest will be written down soon