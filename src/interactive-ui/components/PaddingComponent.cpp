#include <interactive-ui/components/PaddingComponent.h>

PaddingComponent::PaddingComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& size, bool outline, int32_t z_layer, Screen* initial_screen)
    : Component(manager, origin, z_layer, initial_screen), outlined(outline)
{
    draw_dimensions.max = size;
}
PaddingComponent::PaddingComponent(ScreenManager* manager, const Vec2f& screen_percentage, const Vec2i32& size, bool outline, int32_t z_layer, Screen* initial_screen)
    : Component(manager, screen_percentage, z_layer, initial_screen), outlined(outline)
{
    draw_dimensions.max = size;
}

void PaddingComponent::Draw()
{
    display->DrawSquare(origin_position + draw_dimensions.min, draw_dimensions.max - draw_dimensions.min, color);
}