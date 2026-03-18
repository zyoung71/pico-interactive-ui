#include <interactive-ui/ScreenManager.h>
#include <interactive-ui/graphics/Rasterization.h>

void DisplayInterface::DrawCharacter(Vec2i32 pos, char c, const Font& font, uint32_t scale, uint32_t color)
{
    const Vec2i32 scale_vec(scale);

    if (c < font.ascii_begin || c > font.ascii_end)
        return;
    
    uint32_t ppl = (font.char_height >> 3) + (font.char_height & 7) > 0;
    for (uint8_t width = 0; width < font.char_width; width++)
    {
        uint32_t idx = (c - font.ascii_begin) * font.char_width * ppl + width * ppl;
        for (int32_t lp = 0; lp < ppl; lp++)
        {
            uint8_t line = font.char_bitmap[idx];
            for (int8_t j = 0; j < 8; j++, line >>= 1)
            {
                if (line & 1)
                    FillSquare(pos + Vec2i32{width, (lp << 3) + j} * scale, scale_vec, color);
            }
            idx++;
        }
    }
}

void DisplayInterface::DrawText(Vec2i32 pos, const char* text, const Font& font, uint32_t scale, uint32_t color)
{
    for (int32_t x_n = pos.x; *text; x_n += (font.char_width + font.char_height) * scale)
    {
        pos.x = x_n;
        DrawCharacter(pos, *(text++), font, scale, color);
    }
}

void DisplayInterface::DrawLine(Vec2i32 pos_begin, Vec2i32 pos_end, uint32_t color)
{
    if (pos_begin.x == pos_end.x) // if vertical line
    {
        for (; pos_begin.y <= pos_end.y; pos_begin.y++)
            DrawPixel(pos_begin, color);
        return;
    }
    if (pos_begin.y == pos_end.y) // if horizontal line
    {
        for (; pos_begin.x <= pos_end.x; pos_begin.x++)
            DrawPixel(pos_begin, color);
        return;
    }

    Vec2f delta = static_cast<Vec2f>(pos_end - pos_begin);
    float slope = delta.y / delta.x;

    for (int32_t x = pos_begin.x; x <= pos_end.x; x++)
    {
        float y = slope * static_cast<float>(x - pos_begin.x) + (float)pos_begin.y;
        DrawPixel(Vec2i32{x, (int32_t)y}, color);
    }
}

void DisplayInterface::DrawPolygon(const Vec2i32* points, size_t pos_count, uint32_t color)
{
    Vec2i32 begin, end;
    for (size_t i = 0; i < pos_count; i++)
    {
        begin = points[i];
        end = points[(i + 1) % pos_count];
        DrawLine(begin, end, color);
    }
}

void DisplayInterface::DrawPolygon(const ArrayView<Vec2i32>& points, uint32_t color)
{
    return DrawPolygon(points.data, points.length, color);
}

void DisplayInterface::DrawCircle(Vec2i32 center_pos, Vec2i32 radius, uint32_t color)
{
    Vec2i32 index_pos = {0, radius.y};
    Vec2i32 index_pos_conj;

    Vec2i32 r_sqr = {radius.x * radius.x, radius.y * radius.y};
    Vec2i32 d;
    d.x = 0;
    d.y = 2 * r_sqr.x * index_pos.y;

    int32_t d1 = r_sqr.y - (r_sqr.x * radius.y) + (r_sqr.x / 4);

    while (d.x < d.y)
    {
        index_pos_conj.x = -index_pos.x;
        index_pos_conj.y = index_pos.y;

        DrawPixel(center_pos + index_pos, color);
        DrawPixel(center_pos + index_pos_conj, color);
        DrawPixel(center_pos - index_pos_conj, color);
        DrawPixel(center_pos - index_pos, color);

        index_pos.x++;
        d.x += 2 * r_sqr.y;

        if (d1 < 0)
            d1 += d.x + r_sqr.y;
        else
        {
            index_pos.y--;
            d.y -= 2 * r_sqr.x;
            d1 += d.x - d.y + r_sqr.y;
        }
    }
}

void DisplayInterface::DrawSquare(Vec2i32 pos, Vec2i32 size, uint32_t color)
{
    DrawLine(pos, pos + Vec2i32{size.x, 0}, color);
    DrawLine(pos + Vec2i32{0, size.y}, pos + size, color);
    DrawLine(pos, pos + Vec2i32{0, size.y}, color);
    DrawLine(pos + Vec2i32{size.x, 0}, pos + size, color);
}

void DisplayInterface::DrawSquare(AABBi32 dimensions, uint32_t color)
{
    DrawSquare(dimensions.min, dimensions.max, color);
}

void DisplayInterface::DrawBitmap(const uint8_t* bitmap_buff, size_t bitmap_size)
{
    // todo
}

void DisplayInterface::DrawBitmap(const ArrayView<uint8_t>& bitmap)
{
    DrawBitmap(bitmap.data, bitmap.length);
}

void DisplayInterface::FillPolygon(const Vec2i32* points, size_t pos_count, uint32_t fill_color)
{
    graphics::scanline_rasterization(make_array_view(points, pos_count), this, fill_color);
}

void DisplayInterface::FillPolygon(const ArrayView<Vec2i32>& points, uint32_t color)
{
    graphics::scanline_rasterization(points, this, color);
}

void DisplayInterface::FillCircle(Vec2i32 center_pos, Vec2i32 radius, uint32_t color)
{
    int32_t hh = radius.x * radius.x;
    int32_t ww = radius.y * radius.y;
    int32_t hhww = hh * ww;
    int32_t x_0 = radius.x;
    int32_t dx = 0;

    for (int32_t x = -radius.x; x <= radius.x; x++)
        DrawPixel(center_pos + Vec2i32{x, 0}, color);

    for (int32_t y = 1; y <= radius.y; y++)
    {
        int32_t x_1 = x_0 - (dx - 1);
        for (; x_1 > 0; x_1--)
            if (x_1 * x_1 * hh + y * y * ww <= hhww)    
                break;

        dx = x_0 - x_1;
        x_0 = x_1;

        for (int32_t x = -x_0; x <= x_0; x++)
        {
            DrawPixel(center_pos + Vec2i32{x, -y}, color);
            DrawPixel(center_pos + Vec2i32{x, y}, color);
        }
    }
}   

void DisplayInterface::FillSquare(Vec2i32 pos, Vec2i32 size, uint32_t color)
{
    for (; pos.x < size.x; pos.x++)
        for (; pos.y < size.y; pos.y++)
            DrawPixel(pos, color);
}

void DisplayInterface::FillSquare(AABBi32 dimensions, uint32_t color)
{
    FillSquare(dimensions.min, dimensions.max, color);
}