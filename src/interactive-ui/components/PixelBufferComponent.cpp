#include <interactive-ui/components/PixelBufferComponent.h>

PixelBufferComponent::PixelBufferComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& dimensions, int32_t z_layer, Screen* initial_screen)
    : SelectableComponent(manager, origin, z_layer, initial_screen)
{
    draw_dimensions.max = dimensions;
}

PixelBufferComponent::PixelBufferComponent(ScreenManager* manager, const Vec2f& screen_percentage, const Vec2i32& dimensions, int32_t z_layer, Screen* initial_screen)
    : SelectableComponent(manager, screen_percentage, z_layer, initial_screen)
{
    draw_dimensions.max = dimensions;
}

PixelBufferComponent::PixelBufferComponent(ScreenManager* manager, const Vec2i32& origin, const Pixel* initial_buffer, size_t buff_length, int32_t z_layer, Screen* initial_screen)
    : PixelBufferComponent(manager, origin, Vec2i32{}, z_layer, initial_screen)
{
    SetPixels(initial_buffer, buff_length);
}

PixelBufferComponent::PixelBufferComponent(ScreenManager* manager, const Vec2i32& origin, const ArrayView<Pixel>& initial_buffer, int32_t z_layer, Screen* initial_screen)
    : PixelBufferComponent(manager, origin, Vec2i32{}, z_layer, initial_screen)
{
    SetPixels(initial_buffer);
}

PixelBufferComponent::PixelBufferComponent(ScreenManager* manager, const Vec2f& screen_percentage, const Pixel* initial_buffer, size_t buff_length, Screen* initial_screen)
    : PixelBufferComponent(manager, screen_percentage, Vec2i32{}, z_layer, initial_screen)
{
    SetPixels(initial_buffer, buff_length);
}

PixelBufferComponent::PixelBufferComponent(ScreenManager* manager, const Vec2f& screen_percentage, const ArrayView<Pixel>& initial_buffer, int32_t z_layer, Screen* initial_screen)
    : PixelBufferComponent(manager, screen_percentage, Vec2i32{}, z_layer, initial_screen)
{
    SetPixels(initial_buffer);
}

void PixelBufferComponent::SetPixel(Pixel p)
{
    bool needs_realignment = false;
    if (p.position.x < draw_dimensions.xmin)
    {
        draw_dimensions.xmin = p.position.x;
        needs_realignment = true;
    }
    if (p.position.y < draw_dimensions.ymin)
    {
        draw_dimensions.ymin = p.position.y;
        needs_realignment = true;
    }
    if (p.position.x > draw_dimensions.xmax)
    {
        draw_dimensions.xmax = p.position.x;
        needs_realignment = true;
    }
    if (p.position.y > draw_dimensions.ymax)
    {
        draw_dimensions.ymax = p.position.y;
        needs_realignment = true;
    }

    pixel_map[p.position] = p.color;

    if (needs_realignment)
        Align();
}

void PixelBufferComponent::ClearPixels()
{
    pixel_map.clear();
}

void PixelBufferComponent::SetPixels(const ArrayView<Pixel>& pixels)
{
    int32_t max_x = INT32_MIN, max_y = INT32_MIN, min_x = INT32_MAX, min_y = INT32_MAX;
    for (size_t i = 0; i < pixels.length; i++)
    {
        const Pixel& p = pixels[i];
        pixel_map[p.position] = p.color;
    }
    for (const auto& [p, c] : pixel_map) // another loop must be one to account for old pixel locations
    {
        min_x = std::min(p.x, min_x);
        min_y = std::min(p.y, min_y);
        max_x = std::max(p.x, max_x);
        max_y = std::max(p.y, max_y);
    }
    draw_dimensions.min = Vec2i32{min_x, min_y};
    draw_dimensions.max = Vec2i32{max_x, max_y};
    Align();
}

void PixelBufferComponent::SetPixels(const Pixel* pixels, size_t p_count)
{
    int32_t max_x = INT32_MIN, max_y = INT32_MIN, min_x = INT32_MAX, min_y = INT32_MAX;
    for (size_t i = 0; i < p_count; i++)
    {
        const Pixel& p = pixels[i];
        pixel_map[p.position] = p.color;
    }
    for (const auto& [p, c] : pixel_map)
    {
        min_x = std::min(p.x, min_x);
        min_y = std::min(p.y, min_y);
        max_x = std::max(p.x, max_x);
        max_y = std::max(p.y, max_y);
    }
    draw_dimensions.min = Vec2i32{min_x, min_y};
    draw_dimensions.max = Vec2i32{max_x, max_y};
    Align();
}

void PixelBufferComponent::Draw()
{
    for (const auto& [p, c] : pixel_map)
    {
        display->DrawPixel(p + origin_position + draw_dimensions.min, c);
    }
}