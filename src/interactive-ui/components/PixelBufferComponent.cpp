#include <interactive-ui/components/PixelBufferComponent.h>

PixelBufferComponent::PixelBufferComponent(const ScreenManager* manager, const Vec2i32& origin, const Vec2i32& dimensions, int32_t z_layer, const Screen* initial_screen)
    : SelectableComponent(manager, origin, z_layer, initial_screen)
{
    draw_dimensions = dimensions;
}

PixelBufferComponent::PixelBufferComponent(const ScreenManager* manager, float x_percentage, float y_percentage, const Vec2i32& dimensions, int32_t z_layer, const Screen* initial_screen)
    : SelectableComponent(manager, x_percentage, y_percentage, z_layer, initial_screen)
{
    draw_dimensions = dimensions;
}

PixelBufferComponent::PixelBufferComponent(const ScreenManager* manager, const Vec2i32& origin, const Pixel* initial_buffer, size_t buff_length, int32_t z_layer, const Screen* initial_screen)
    : PixelBufferComponent(manager, origin, Vec2i32{}, z_layer, initial_screen)
{
    int32_t m_x = 0, m_y = 0;
    for (size_t i = 0; i < buff_length; i++)
    {
        const Pixel& p = initial_buffer[i];
        pixels[p.position] = p.color;
        m_x = p.position.x > m_x ? p.position.x : m_x;
        m_y = p.position.y > m_y ? p.position.y : m_y;
    }
    draw_dimensions = Vec2i32{m_x, m_y};
}

PixelBufferComponent::PixelBufferComponent(const ScreenManager* manager, const Vec2i32& origin, const ArrayView<Pixel>& initial_buffer, int32_t z_layer, const Screen* initial_screen)
    : PixelBufferComponent(manager, origin, Vec2i32{}, z_layer, initial_screen)
{
    int32_t m_x = 0, m_y = 0;
    for (size_t i = 0; i < initial_buffer.length; i++)
    {
        const Pixel& p = initial_buffer[i];
        pixels[p.position] = p.color;
        m_x = p.position.x > m_x ? p.position.x : m_x;
        m_y = p.position.y > m_y ? p.position.y : m_y;
    }
    draw_dimensions = Vec2i32{m_x, m_y};
}

PixelBufferComponent::PixelBufferComponent(const ScreenManager* manager, float x_percentage, float y_percentage, const Pixel* initial_buffer, size_t buff_length, const Screen* initial_screen)
    : PixelBufferComponent(manager, x_percentage, y_percentage, Vec2i32{}, z_layer, initial_screen)
{
    int32_t m_x = 0, m_y = 0;
    for (size_t i = 0; i < buff_length; i++)
    {
        const Pixel& p = initial_buffer[i];
        pixels[p.position] = p.color;
        m_x = p.position.x > m_x ? p.position.x : m_x;
        m_y = p.position.y > m_y ? p.position.y : m_y;
    }
    draw_dimensions = Vec2i32{m_x, m_y};
}

PixelBufferComponent::PixelBufferComponent(const ScreenManager* manager, float x_percentage, float y_percentage, const ArrayView<Pixel>& initial_buffer, int32_t z_layer, const Screen* initial_screen)
    : PixelBufferComponent(manager, x_percentage, y_percentage, Vec2i32{}, z_layer, initial_screen)
{
    int32_t m_x = 0, m_y = 0;
    for (size_t i = 0; i < initial_buffer.length; i++)
    {
        const Pixel& p = initial_buffer[i];
        pixels[p.position] = p.color;
        m_x = p.position.x > m_x ? p.position.x : m_x;
        m_y = p.position.y > m_y ? p.position.y : m_y;
    }
    draw_dimensions = Vec2i32{m_x, m_y};
}

void PixelBufferComponent::SetPixel(Pixel p)
{
    p.position.x = p.position.x > draw_dimensions.x ? draw_dimensions.x : p.position.x;
    p.position.y = p.position.y > draw_dimensions.y ? draw_dimensions.y : p.position.y;
    pixels[p.position] = p.color;
}

void PixelBufferComponent::Draw()
{
    for (const auto& [p, c] : pixels)
    {
        display->DrawPixel(p + origin_position, c);
    }
}