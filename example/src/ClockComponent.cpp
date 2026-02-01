#include <ClockComponent.h>

ClockComponent::ClockComponent(ScreenManager* manager, RealTimeClock* clock, int32_t z_layer, Screen* initial_screen)
    : TextComponent(manager, 1.0, 0.1, clock->GetTimeString(), &clock_font, z_layer, initial_screen), clock(clock) // Aiming for top-right corner. 100% x-axis, 10% y-axis.
{
    Align();
}

void ClockComponent::Update(float dt, const Screen* screen)
{
    Align();
    Component::Update(dt, screen);
    clock->UpdateDateAndTime();
}