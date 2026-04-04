#include <interactive-ui/ScreenManager.h>
#include <interactive-ui/graphics/Rasterization.h>

void DisplayInterface::DrawCharacter(Vec2i32 pos, char c, const Font& font, uint32_t scale, RGBA color)
{
    const Vec2i32 scale_vec(scale);

    if (c < font.ascii_begin || c > font.ascii_end)
        return;

    uint32_t ppl = (font.char_height >> 3) + ((font.char_height & 7) > 0);
    for (int32_t width = 0; width < font.char_width; width++)
    {
        uint32_t idx = (c - font.ascii_begin) * font.char_width * ppl + width * ppl;
        for (int32_t lp = 0; lp < ppl; lp++)
        {
            uint8_t line = font.char_bitmap[idx];
            for (int32_t j = 0; j < 8; j++, line >>= 1)
            {
                if (line & 1)
                    FillRectangle(pos + Vec2i32{width, (lp << 3) + j} * scale, scale_vec, color);
            }
            idx++;
        }
    }
}

void DisplayInterface::DrawText(Vec2i32 pos, const char* text, const Font& font, uint32_t scale, RGBA color)
{
    for (int32_t x_n = pos.x; *text; x_n += (font.char_width + font.char_height) * scale)
    {
        pos.x = x_n;
        DrawCharacter(pos, *(text++), font, scale, color);
    }
}

void DisplayInterface::DrawHorizontalLine(Vec2i32 pos_begin, int32_t length, RGBA color)
{
    int32_t x_end = pos_begin.x + length;
    for (int32_t x = pos_begin.x; x <= x_end; x++)
        DrawPixel(x, pos_begin.y, color);
}

void DisplayInterface::DrawHorizontalLineX1X2(int32_t x1, int32_t x2, int32_t y, RGBA color)
{
    for (; x1 <= x2; x1++)
        DrawPixel(x1, y, color);
}

void DisplayInterface::DrawVerticalLineY1Y2(int32_t y1, int32_t y2, int32_t x, RGBA color)
{
    for (; y1 <= y2; y1++)
        DrawPixel(x, y1, color);
}

void DisplayInterface::DrawVerticalLine(Vec2i32 pos_begin, int32_t length, RGBA color)
{
    int32_t y_end = pos_begin.y + length;
    for (int32_t y = pos_begin.y; y <= y_end; y++)
        DrawPixel(pos_begin.x, y, color);
}

void DisplayInterface::DrawLine(Vec2i32 pos_begin, Vec2i32 pos_end, RGBA color)
{
    Vec2f delta = static_cast<Vec2f>(pos_end - pos_begin);
    float slope = delta.y / delta.x;
    float y;

    for (int32_t x = pos_begin.x; x <= pos_end.x; x++)
    {
        y = slope * static_cast<float>(x - pos_begin.x) + (float)pos_begin.y;
        DrawPixel(Vec2i32{x, (int32_t)y}, color);
    }
}

void DisplayInterface::DrawPolygon(const Vec2i32* points, size_t pos_count, RGBA color)
{
    Vec2i32 begin, end;
    for (size_t i = 0; i < pos_count; i++)
    {
        begin = points[i];
        end = points[(i + 1) % pos_count];
        DrawLine(begin, end, color);
    }
}

void DisplayInterface::DrawPolygon(const ArrayView<Vec2i32>& points, RGBA color)
{
    return DrawPolygon(points.data, points.length, color);
}

void DisplayInterface::DrawCircle(Vec2i32 center_pos, int32_t radius, RGBA color)
{
    Vec2i32 draw_pos = {0, radius};
    int32_t decision = 1 - radius;

    Vec2i32 conj, flip;
    while (draw_pos.x < draw_pos.y)
    {
        conj.x = -draw_pos.x;
        conj.y = draw_pos.y;
        DrawPixel(center_pos + draw_pos, color);
        DrawPixel(center_pos + conj, color);
        DrawPixel(center_pos - conj, color);
        DrawPixel(center_pos - draw_pos, color);

        flip = {draw_pos.y, draw_pos.x};
        conj.x = -flip.x;
        conj.y = flip.y;
        DrawPixel(center_pos + flip, color);
        DrawPixel(center_pos + conj, color);
        DrawPixel(center_pos - conj, color);
        DrawPixel(center_pos - flip, color);

        if (decision < 0)
            decision += 2 * draw_pos.x + 3;
        else
        {
            decision += 2 * (draw_pos.x - draw_pos.y) + 5;
            draw_pos.y--;
        }
        draw_pos.x++;
    }
}

void DisplayInterface::DrawEllipse(Vec2i32 center_pos, Vec2i32 radius, RGBA color)
{
    Vec2i32 r_sqr = radius * radius * 2;
    Vec2i32 r_sqr_times_2 = r_sqr * 2;
    Vec2i32 draw_pos = {0, radius.y};
    Vec2i32 delta = {0, r_sqr_times_2.y * radius.y};

    int32_t decision = r_sqr.y - r_sqr.x * radius.y + (r_sqr.x >> 2);

    Vec2i32 conj;
    while (delta.x < delta.y)
    {
        conj.x = -draw_pos.x;
        conj.y = draw_pos.y;
        DrawPixel(center_pos + draw_pos, color);
        DrawPixel(center_pos + conj, color);
        DrawPixel(center_pos - conj, color);
        DrawPixel(center_pos - draw_pos, color);

        draw_pos.x++;
        delta.x += r_sqr_times_2.y;
        
        if (decision < 0)
            decision += delta.x + r_sqr.y;
        else
        {
            draw_pos.y--;
            delta.y -= r_sqr_times_2.x;
            decision += delta.x - delta.y + r_sqr.y;
        }
    }

    decision = r_sqr.y * draw_pos.x * draw_pos.x + r_sqr.x * (draw_pos.y - 1) * (draw_pos.y - 1) - r_sqr.x * r_sqr.y;
    while (draw_pos.y >= 0)
    {
        conj.x = -draw_pos.x;
        conj.y = draw_pos.y;
        DrawPixel(center_pos + draw_pos, color);
        DrawPixel(center_pos + conj, color);
        DrawPixel(center_pos - conj, color);
        DrawPixel(center_pos - draw_pos, color);

        draw_pos.y--;
        delta.y -= r_sqr_times_2.x;

        if (decision > 0)
            decision += r_sqr.x - delta.y;
        else
        {
            draw_pos.x++;
            delta.x += r_sqr_times_2.y;
            decision += delta.x - delta.y + r_sqr.x;
        }
    }
}

void DisplayInterface::DrawRectangle(Vec2i32 pos, Vec2i32 size, RGBA color)
{
    DrawHorizontalLine(pos, size.x, color);
    DrawVerticalLine(pos, size.y, color);
    pos.y += size.y;
    DrawHorizontalLine(pos, size.x, color);
    pos.x += size.x;
    DrawVerticalLine(pos, size.y, color);
}

void DisplayInterface::DrawRectangle(AABBi32 dimensions, RGBA color)
{
    DrawRectangle(dimensions.min, dimensions.max, color);
}

void DisplayInterface::DrawRoundedRectangle(Vec2i32 pos, Vec2i32 size, Vec2i32 radius, RGBA color)
{
    Vec2i32 begin = pos + radius;
    Vec2i32 end = pos + size - radius - Vec2i32(1);
    
    DrawHorizontalLineX1X2(begin.x, end.x, begin.y, color);
    DrawHorizontalLineX1X2(begin.x, end.x, end.y, color);
    DrawVerticalLineY1Y2(begin.y, end.y, begin.x, color);
    DrawVerticalLineY1Y2(begin.y, end.y, end.x, color);

    Vec2i32 r_sqr = radius * radius;
    Vec2i32 r_sqr_times_2 = r_sqr * 2;

    Vec2i32 draw_offset = {0, radius.y};
    Vec2i32 draw_offset_conj;
    Vec2i32 delta = {0, r_sqr_times_2.x * radius.y};

    int32_t decision = r_sqr.y - r_sqr.x * radius.y + (r_sqr.x >> 2);
    while (delta.x < delta.y)
    {
        draw_offset_conj.x = -draw_offset.x;
        draw_offset_conj.y = draw_offset.y;
        DrawPixel(begin + draw_offset, color);
        DrawPixel(begin + draw_offset_conj, color);
        DrawPixel(begin - draw_offset_conj, color);
        DrawPixel(begin - draw_offset, color);

        draw_offset.x++;
        delta.x += r_sqr_times_2.y;

        if (decision < 0)
            decision += delta.x + r_sqr.y;
        else
        {
            draw_offset.x--;
            delta.y -= r_sqr_times_2.x;
            decision += delta.x - delta.y + r_sqr.y;
        }
    }

    decision = r_sqr.y * draw_offset.x * draw_offset.x + r_sqr.x * (draw_offset.y - 1) * (draw_offset.y - 1) - r_sqr.x * r_sqr.y;
    while (draw_offset.y >= 0)
    {
        draw_offset_conj.x = -draw_offset.x;
        draw_offset_conj.y = draw_offset.y;
        DrawPixel(begin + draw_offset, color);
        DrawPixel(begin + draw_offset_conj, color);
        DrawPixel(begin - draw_offset_conj, color);
        DrawPixel(begin - draw_offset, color);

        draw_offset.y--;
        delta.y -= r_sqr_times_2.x;

        if (decision > 0)
            decision += r_sqr.x - delta.y;
        else
        {
            draw_offset.x++;
            delta.x += r_sqr_times_2.y;
            decision += delta.x - delta.y + r_sqr.x;
        }
    }
}

void DisplayInterface::DrawRoundedRectangle(AABBi32 dimensions, Vec2i32 radius, RGBA color)
{
    DrawRoundedRectangle(dimensions.min, dimensions.max, radius, color);
}

void DisplayInterface::FillPolygon(const Vec2i32* points, size_t pos_count, RGBA color)
{
    graphics::scanline_rasterization(make_array_view(points, pos_count), this, color);
}

void DisplayInterface::FillPolygon(const ArrayView<Vec2i32>& points, RGBA color)
{
    graphics::scanline_rasterization(points, this, color);
}

void DisplayInterface::FillCircle(Vec2i32 center_pos, int32_t radius, RGBA color)
{
    int32_t x = 0;
    int32_t decision = 1 - radius;
    while (x <= radius)
    {
        DrawHorizontalLine(center_pos + Vec2i32{-x, radius}, x, color);
        DrawHorizontalLine(center_pos - Vec2i32{x, radius}, x, color);
        DrawHorizontalLine(center_pos + Vec2i32{-radius, x}, radius, color);
        DrawHorizontalLine(center_pos - Vec2i32{radius, x}, radius, color);

        if (decision < 0)
            decision += 2 * x + 3;
        else
        {
            decision += 2 * (x - radius) + 5;
            radius--;
        }

        x++;
    }
}

void DisplayInterface::FillEllipse(Vec2i32 center_pos, Vec2i32 radius, RGBA color)
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

void DisplayInterface::FillRectangle(Vec2i32 pos, Vec2i32 size, RGBA color)
{
    for (; pos.x < size.x; pos.x++)
        for (; pos.y < size.y; pos.y++)
            DrawPixel(pos, color);
}

void DisplayInterface::FillRectangle(AABBi32 dimensions, RGBA color)
{
    FillRectangle(dimensions.min, dimensions.max, color);
}