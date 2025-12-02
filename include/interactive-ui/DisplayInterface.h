#pragma once

#include <math/Vec2.h>

class DisplayInterface
{
public:
    virtual ~DisplayInterface() = default;

    virtual void DrawText(Vec2u32 pos, const char* str, uint32_t color) = 0;
    virtual void DrawPixel(Vec2u32 pos, uint32_t color) = 0;
    virtual void DrawLine(Vec2u32 pos_begin, Vec2u32 pos_end, uint32_t color) = 0;
    virtual void DrawPolygon(const Vec2u32* points, size_t pos_count, uint32_t color, bool is_outline = true) = 0;
    virtual void DrawSquare(Vec2u32 pos, Vec2u32 size, uint32_t color, bool is_outline = true) = 0;
    virtual void DisplayBitmap(const uint8_t* bitmap_buff, size_t bitmap_size) = 0;
};