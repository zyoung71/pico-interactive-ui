#include <interactive-ui/components/BitmapComponent.h>

BitmapComponent::BitmapComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& dimensions, uint32_t* pixels, int32_t z_layer, const Screen* initial_screen)
    : SelectableComponent(manager, origin, z_layer, initial_screen), pixel_map(make_array_view(pixels, dimensions.y, dimensions.x))
{
    draw_dimensions = dimensions;
}

BitmapComponent::BitmapComponent(ScreenManager* manager, float x_percentage, float y_percentage, const Vec2i32& dimensions, uint32_t* pixels, int32_t z_layer, const Screen* initial_screen)
    : SelectableComponent(manager, x_percentage, y_percentage, z_layer, initial_screen), pixel_map(make_array_view(pixels, dimensions.y, dimensions.x))
{
    draw_dimensions = dimensions;
}

BitmapComponent::BitmapComponent(ScreenManager* manager, const Vec2i32& origin, const ArrayView2D<uint32_t>& pixels, int32_t z_layer, const Screen* initial_screen)
    : SelectableComponent(manager, origin, z_layer, initial_screen), pixel_map(pixels)
{
    draw_dimensions = Vec2i32{(int32_t)pixels.cols, (int32_t)pixels.rows};
}

BitmapComponent::BitmapComponent(ScreenManager* manager, float x_percentage, float y_percentage, const ArrayView2D<uint32_t>& pixels, int32_t z_layer, const Screen* initial_screen)
    : SelectableComponent(manager, x_percentage, y_percentage, z_layer, initial_screen)
{
    draw_dimensions = Vec2i32{(int32_t)pixels.cols, (int32_t)pixels.rows};
}

void BitmapComponent::Draw()
{
    Vec2i32 end = origin_position + draw_dimensions;
    int32_t x_0 = 0, y_0 = 0; // Bitmap's local coords
    for (int32_t y = origin_position.y; y < end.y; y++)
    {
        for (int32_t x = origin_position.x; x < end.x; x++)
        {
            display->DrawPixel(Vec2i32{x, y}, pixel_map(y_0, x_0));
            x_0++;
        }
        x_0 = 0;
        y_0++;
    }
}