#pragma once

#include <interactive-ui/components/TextComponent.h>
#include <rtc/RealTimeClock.h>

constexpr uint8_t data[] = {}; // Font text is implemented here. It will be long.
constexpr Font clock_font = Font::BuildFont<8, 5, 1>(data);

// Non-selectable clock in the corner of the screen.
class ClockComponent : public TextComponent
{
private:
    RealTimeClock* clock;

public:
    ClockComponent(ScreenManager* screen, RealTimeClock* clock, int32_t z_layer, Screen* initial_screen);

    void Update(float dt) override;
    void Draw() override;
};