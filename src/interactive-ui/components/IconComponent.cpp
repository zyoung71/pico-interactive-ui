#include <interactive-ui/components/IconComponent.h>

IconComponent::IconComponent(Screen* screen, const Vec2u32& origin, const Vec2u32& dimensions, uint32_t* pixels, int32_t z_layer)
    : SelectableComponent(screen, origin, z_layer), pixel_map(make_array_view(pixels, dimensions.y, dimensions.x))
{
    draw_dimensions = dimensions;
}

IconComponent::IconComponent(Screen* screen, float x_percentage, float y_percentage, const Vec2u32& dimensions, uint32_t* pixels, int32_t z_layer)
    : SelectableComponent(screen, x_percentage, y_percentage, z_layer), pixel_map(make_array_view(pixels, dimensions.y, dimensions.x))
{
    draw_dimensions = dimensions;
}

IconComponent::IconComponent(Screen* screen, const Vec2u32& origin, const ArrayView2D<uint32_t>& pixels, int32_t z_layer)
    : SelectableComponent(screen, origin, z_layer), pixel_map(pixels)
{
    draw_dimensions = Vec2u32{pixels.cols, pixels.rows};
}

IconComponent::IconComponent(Screen* screen, float x_percentage, float y_percentage, const ArrayView2D<uint32_t>& pixels, int32_t z_layer)
    : SelectableComponent(screen, x_percentage, y_percentage, z_layer)
{
    draw_dimensions = Vec2u32{pixels.cols, pixels.rows};
}

void IconComponent::Draw()
{
    Vec2u32 end = origin_position + draw_dimensions;
    for (uint32_t y = origin_position.y; y < end.y; y++)
    {
        for (uint32_t x = origin_position.x; x < end.x; x++)
        {
            data.display->DrawPixel({x, y}, pixel_map(y, x));
        }
    }
}