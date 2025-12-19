#include <ClockComponent.h>

ClockComponent::ClockComponent(const ScreenManager* manager, RealTimeClock* clock, int32_t z_layer, const Screen* initial_screen)
    : TextComponent(manager, 1.0, 0.1, clock->GetTimeString(), &clock_font, z_layer, initial_screen), clock(clock) // Aiming for top-right corner. 100% x-axis, 10% y-axis.
{
    draw_dimensions = {30, 6};
}

void ClockComponent::Update(float dt)
{
    Component::Update(dt);
    clock->UpdateDateAndTime();
}

void ClockComponent::Draw()
{
    // Here, the screen we will want to use will be implemented already.
    // The origin position borders the right wall, and a bit downward from the top.
    // The origin of drawing should be the top left corner of the component, so subtract the draw dimensions.
    display->DrawText(origin_position - draw_dimensions, text, font, 0xFFFFFFFF);
}