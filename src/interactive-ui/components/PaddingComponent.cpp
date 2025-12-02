#include <interactive-ui/components/PaddingComponent.h>

PaddingComponent::PaddingComponent(Screen* screen, const Vec2u32& origin, const Vec2u32& size, bool outline, int32_t z_layer)
    : Component(screen, origin, z_layer), outlined(outline)
{
    draw_dimensions = size;
}
PaddingComponent::PaddingComponent(Screen* screen, float x_percentage, float y_percentage, const Vec2u32& size, bool outline, int32_t z_layer)
    : Component(screen, x_percentage, y_percentage, z_layer), outlined(outline)
{
    draw_dimensions = size;
}

void PaddingComponent::Draw()
{
    data.display->DrawSquare(origin_position, draw_dimensions, color);
}