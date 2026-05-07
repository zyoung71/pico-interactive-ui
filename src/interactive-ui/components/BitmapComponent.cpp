#include <interactive-ui/components/BitmapComponent.h>

BitmapComponent::BitmapComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& dimensions, RGBA* pixels, int32_t z_layer, Screen* initial_screen)
    : SelectableComponent(manager, origin, z_layer, initial_screen), pixel_map(make_array_view(pixels, dimensions.y, dimensions.x))
{
    draw_dimensions.max = dimensions;
}

BitmapComponent::BitmapComponent(ScreenManager* manager, const Vec2f& screen_percentage, const Vec2i32& dimensions, RGBA* pixels, int32_t z_layer, Screen* initial_screen)
    : SelectableComponent(manager, screen_percentage, z_layer, initial_screen), pixel_map(make_array_view(pixels, dimensions.y, dimensions.x))
{
    draw_dimensions.max = dimensions;
}

BitmapComponent::BitmapComponent(ScreenManager* manager, const Vec2i32& origin, const ArrayView2D<RGBA>& pixels, int32_t z_layer, Screen* initial_screen)
    : SelectableComponent(manager, origin, z_layer, initial_screen), pixel_map(pixels)
{
    draw_dimensions.max = Vec2i32{(int32_t)pixels.cols, (int32_t)pixels.rows};
}

BitmapComponent::BitmapComponent(ScreenManager* manager, const Vec2f& screen_percentage, const ArrayView2D<RGBA>& pixels, int32_t z_layer, Screen* initial_screen)
    : SelectableComponent(manager, screen_percentage, z_layer, initial_screen)
{
    draw_dimensions.max = Vec2i32{(int32_t)pixels.cols, (int32_t)pixels.rows};
}

void BitmapComponent::Draw(const Screen* screen)
{
    if (scale == 0)
        return;

    Vec2i32 ss_pos = screen->ToScreenCoords(origin_position);

    Vec2i32 end = ss_pos + draw_dimensions.Size();
    int32_t x_0 = 0, y_0 = 0; // Bitmap's local coords
    
    // so... much... boilerplate... all for optimization
    if (mirror_horizontally)
    {
        if (mirror_vertically)
        {
            if (override_color)
            {
                RGBA adjusted = color;
                for (int32_t y = end.y - 1 + draw_dimensions.ymin; y >= ss_pos.y; y -= scale)
                {
                    for (int32_t x = end.x - 1 + draw_dimensions.xmin; x >= ss_pos.x; x -= scale)
                    {
                        adjusted.alpha = pixel_map(y_0, x_0).alpha;
                        display->FillRectangle(x, y, scale, scale, adjusted);
                            
                        x_0++;
                    }
                    x_0 = 0;
                    y_0++;
                }
            }
            else
            {
                for (int32_t y = end.y - 1 + draw_dimensions.ymin; y >= ss_pos.y; y -= scale)
                {
                    for (int32_t x = end.x - 1 + draw_dimensions.xmin; x >= ss_pos.x; x -= scale)
                    {
                        display->FillRectangle(x, y, scale, scale, pixel_map(y_0, x_0));
                        x_0++;
                    }
                    x_0 = 0;
                    y_0++;
                }
            }
        }
        else
        {
            if (override_color)
            {
                RGBA adjusted = color;
                for (int32_t y = ss_pos.y + draw_dimensions.ymin; y < end.y; y += scale)
                {
                    for (int32_t x = end.x - 1 + draw_dimensions.xmin; x >= ss_pos.x; x -= scale)
                    {
                        adjusted.alpha = pixel_map(y_0, x_0).alpha;
                        display->FillRectangle(x, y, scale, scale, adjusted);
                            
                        x_0++;
                    }
                    x_0 = 0;
                    y_0++;
                }
            }
            else
            {
                for (int32_t y = ss_pos.y + draw_dimensions.ymin; y < end.y; y += scale)
                {
                    for (int32_t x = end.x - 1 + draw_dimensions.xmin; x >= ss_pos.x; x -= scale)
                    {
                        display->FillRectangle(x, y, scale, scale, pixel_map(y_0, x_0));
                        x_0++;
                    }
                    x_0 = 0;
                    y_0++;
                }
            }
        }
    }
    else
    {
        if (mirror_vertically)
        {
            if (override_color)
            {
                RGBA adjusted = color;
                for (int32_t y = end.y - 1 + draw_dimensions.ymin; y >= ss_pos.y; y -= scale)
                {
                    for (int32_t x = ss_pos.x + draw_dimensions.xmin; x < end.x; x += scale)
                    {
                        adjusted.alpha = pixel_map(y_0, x_0).alpha;
                        display->FillRectangle(x, y, scale, scale, adjusted);
                            
                        x_0++;
                    }
                    x_0 = 0;
                    y_0++;
                }
            }
            else
            {
                for (int32_t y = end.y - 1 + draw_dimensions.ymin; y >= ss_pos.y; y -= scale)
                {
                    for (int32_t x = ss_pos.x + draw_dimensions.xmin; x < end.x; x += scale)
                    {
                        display->FillRectangle(x, y, scale, scale, pixel_map(y_0, x_0));
                        x_0++;
                    }
                    x_0 = 0;
                    y_0++;
                }
            }
        }
        else
        {
            if (override_color)
            {
                RGBA adjusted = color;
                for (int32_t y = ss_pos.y + draw_dimensions.ymin; y < end.y; y += scale)
                {
                    for (int32_t x = ss_pos.x + draw_dimensions.xmin; x < end.x; x += scale)
                    {
                        adjusted.alpha = pixel_map(y_0, x_0).alpha;
                        display->FillRectangle(x, y, scale, scale, adjusted);
                            
                        x_0++;
                    }
                    x_0 = 0;
                    y_0++;
                }
            }
            else
            {
                for (int32_t y = ss_pos.y + draw_dimensions.ymin; y < end.y; y += scale)
                {
                    for (int32_t x = ss_pos.x + draw_dimensions.xmin; x < end.x; x += scale)
                    {
                        display->FillRectangle(x, y, scale, scale, pixel_map(y_0, x_0));
                        x_0++;
                    }
                    x_0 = 0;
                    y_0++;
                }
            }
        }
    }
}