#include <interactive-ui/components/CircleComponent.h>

CircleComponent::CircleComponent(ScreenManager* manager, const Vec2i32& origin, uint32_t radius, int32_t z_layer, Screen* initial_screen)
    : Component(manager, origin, z_layer, initial_screen), radius(radius)
{

}

CircleComponent::CircleComponent(ScreenManager* manager, const Vec2f& percentage, uint32_t radius, int32_t z_layer, Screen* initial_screen)
    : Component(manager, percentage, z_layer, initial_screen), radius(radius)
{
    
}

void CircleComponent::Draw()
{
    const uint32_t half = thickness >> 1;
    uint32_t in, out, in_sqr, out_sqr, d;
    in = radius - half;
    out = radius + half;
    in_sqr = in * in;
    out_sqr = out * out;

    for (int32_t y = -out; y <= out; y++)
    {
        for (int32_t x = -out; x <= out; x++)
        {
            d = x*x + y*y;
            if (d >= in_sqr && d <= out_sqr)
                display->DrawPixel(origin_position + draw_dimensions.min + Vec2i32{x, y}, color);
        }
    }
}