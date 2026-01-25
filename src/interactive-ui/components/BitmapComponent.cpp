#include <interactive-ui/components/BitmapComponent.h>

BitmapComponent::BitmapComponent(const ScreenManager* manager, const Vec2u32& origin, const Vec2u32& dimensions, uint32_t* pixels, int32_t z_layer, const Screen* initial_screen)
    : SelectableComponent(manager, origin, z_layer, initial_screen), pixel_map(make_array_view(pixels, dimensions.y, dimensions.x))
{
    draw_dimensions = dimensions;
}

BitmapComponent::BitmapComponent(const ScreenManager* manager, float x_percentage, float y_percentage, const Vec2u32& dimensions, uint32_t* pixels, int32_t z_layer, const Screen* initial_screen)
    : SelectableComponent(manager, x_percentage, y_percentage, z_layer, initial_screen), pixel_map(make_array_view(pixels, dimensions.y, dimensions.x))
{
    draw_dimensions = dimensions;
}

BitmapComponent::BitmapComponent(const ScreenManager* manager, const Vec2u32& origin, const ArrayView2D<uint32_t>& pixels, int32_t z_layer, const Screen* initial_screen)
    : SelectableComponent(manager, origin, z_layer, initial_screen), pixel_map(pixels)
{
    draw_dimensions = Vec2u32{pixels.cols, pixels.rows};
}

BitmapComponent::BitmapComponent(const ScreenManager* manager, float x_percentage, float y_percentage, const ArrayView2D<uint32_t>& pixels, int32_t z_layer, const Screen* initial_screen)
    : SelectableComponent(manager, x_percentage, y_percentage, z_layer, initial_screen)
{
    draw_dimensions = Vec2u32{pixels.cols, pixels.rows};
}

void BitmapComponent::Draw()
{
    Vec2u32 end = origin_position + draw_dimensions;
    uint32_t x_0 = 0, y_0 = 0; // Bitmap's local coords
    for (uint32_t y = origin_position.y; y < end.y; y++)
    {
        for (uint32_t x = origin_position.x; x < end.x; x++)
        {
            display->DrawPixel(Vec2u32{x, y}, pixel_map(y_0, x_0));
            x_0++;
        }
        x_0 = 0;
        y_0++;
    }
}