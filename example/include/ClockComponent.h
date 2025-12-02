#pragma once

#include <interactive-ui/components/TextComponent.h>
#include <rtc/RealTimeClock.h>

// Non-selectable clock in the corner of the screen.
class ClockComponent : public TextComponent
{
private:
    RealTimeClock* clock;

public:
    ClockComponent(Screen* screen, RealTimeClock* clock, int32_t z_layer);

    void Update(float dt) override;
    void Draw() override;
};