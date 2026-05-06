#include <interactive-ui/components/PaddingComponent.h>

PaddingComponent::PaddingComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& size, int32_t z_layer, Screen* initial_screen)
    : Component(manager, origin, z_layer, initial_screen), fill_color(color)
{
    draw_dimensions.max = size;
}
PaddingComponent::PaddingComponent(ScreenManager* manager, const Vec2f& screen_percentage, const Vec2i32& size, int32_t z_layer, Screen* initial_screen)
    : Component(manager, screen_percentage, z_layer, initial_screen), fill_color(color)
{
    draw_dimensions.max = size;
}

void PaddingComponent::Draw(const Screen* screen)
{
    Vec2i32 ss_pos = screen->ToScreenCoords(origin_position);

    if (!outlined)
    {
        return display->FillRectangle(ss_pos + draw_dimensions.min, draw_dimensions.Size(), color);
    }
    // if there is a background fill, fill it once and draw the next thickness lines inside
    if (fill_if_outlined)
        display->FillRectangle(ss_pos + draw_dimensions.min, draw_dimensions.Size(), color);
    
    Vec2i32 offset;
    for (int32_t i = 0; i < thickness; i++)
    {
        offset.x = i;
        offset.y = i;
        display->DrawRectangle(ss_pos + draw_dimensions.min + offset, draw_dimensions.Size() - offset, color);
    }
}