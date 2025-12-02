#include <interactive-ui/components/IconComponent.h>

IconComponent::IconComponent(Screen* screen, const Vec2u32 &origin, const Vec2u32& dimensions, uint32_t* bmp, int32_t z_layer)
    : SelectableComponent(screen, origin, z_layer), bitmap(std::make_unique<uint32_t[]>(dimensions.x * dimensions.y))
{
    draw_dimensions = dimensions;
    std::move(bmp, bmp + dimensions.x * dimensions.y - 1, bitmap.get());
}

IconComponent::IconComponent(Screen* screen, float x_percentage, float y_percentage, const Vec2u32& dimensions, uint32_t* bmp, int32_t z_layer)
    : SelectableComponent(screen, x_percentage, y_percentage, z_layer), bitmap(std::make_unique<uint32_t[]>(dimensions.x * dimensions.y))
{
    draw_dimensions = dimensions;
    std::move(bmp, bmp + dimensions.x * dimensions.y - 1, bitmap.get());
}

void IconComponent::Draw()
{
    Vec2u32 end = origin_position + draw_dimensions;
    for (uint32_t x = origin_position.x; x < end.x; x++)
    {
        for (uint32_t y = origin_position.y; y < end.y; y++)
        {
            data.display->DrawPixel({x, y}, color);
        }
    }
}