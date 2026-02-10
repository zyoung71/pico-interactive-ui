#include <interactive-ui/components/PaddingComponent.h>

PaddingComponent::PaddingComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& size, int32_t z_layer, Screen* initial_screen)
    : Component(manager, origin, z_layer, initial_screen)
{
    draw_dimensions.max = size;
}
PaddingComponent::PaddingComponent(ScreenManager* manager, const Vec2f& screen_percentage, const Vec2i32& size, int32_t z_layer, Screen* initial_screen)
    : Component(manager, screen_percentage, z_layer, initial_screen)
{
    draw_dimensions.max = size;
}

void PaddingComponent::Draw()
{
    if (!outlined)
    {
        return display->DrawSquare(origin_position + draw_dimensions.min, draw_dimensions.Size(), color, false);
    }
    // if there is a background fill, fill it once and draw the next thickness lines inside
    display->DrawSquare(origin_position + draw_dimensions.min, draw_dimensions.Size(), color, true, fill_if_outlined);
    for (int32_t i = 1; i < thickness; i++)
        display->DrawSquare(origin_position + draw_dimensions.min + Vec2i32{i, i}, draw_dimensions.Size() - Vec2i32{i, i}, color, true, false);
}