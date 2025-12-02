#pragma once

#include <interactive-ui/Component.h>
#include <rtc/RealTimeClock.h>

// Non-selectable clock in the corner of the screen.
class ClockComponent : public Component
{
private:
    RealTimeClock* clock;

public:
    ClockComponent(Screen* screen, RealTimeClock* clock);

    void Update(float dt) override;
    void Draw() override;
};