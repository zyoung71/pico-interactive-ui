#include <interactive-ui/components/PaddingComponent.h>

PaddingComponent::PaddingComponent(const ScreenManager* manager, const Vec2u32& origin, const Vec2u32& size, bool outline, int32_t z_layer, const Screen* initial_screen)
    : Component(manager, origin, z_layer, initial_screen), outlined(outline)
{
    draw_dimensions = size;
}
PaddingComponent::PaddingComponent(const ScreenManager* manager, float x_percentage, float y_percentage, const Vec2u32& size, bool outline, int32_t z_layer, const Screen* initial_screen)
    : Component(manager, x_percentage, y_percentage, z_layer, initial_screen), outlined(outline)
{
    draw_dimensions = size;
}

void PaddingComponent::Draw()
{
    display->DrawSquare(origin_position, draw_dimensions, color);
}