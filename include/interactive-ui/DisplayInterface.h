#pragma once

#include <math/Vec2.h>
#include "Font.h"

class DisplayInterface
{
public:
    virtual ~DisplayInterface() = default;

    virtual void UpdateDisplay() = 0;
    virtual void Power(bool power_on) = 0;
    
    virtual void DrawText(Vec2i32 pos, const char* text, const Font* font, uint32_t color) = 0;
    virtual void DrawPixel(Vec2i32 pos, uint32_t color) = 0;
    virtual void DrawLine(Vec2i32 pos_begin, Vec2i32 pos_end, uint32_t color) = 0;
    virtual void DrawPolygon(const Vec2i32* points, size_t pos_count, uint32_t color, bool is_outline = true) = 0;
    virtual void DrawSquare(Vec2i32 pos, Vec2i32 size, uint32_t color, bool is_outline = true, bool fill_if_outline = false) = 0;

    virtual void DrawScrollingText(Vec2i32 pos, const char* text, bool move_left, const Font* font, uint32_t color) = 0;

    virtual void DisplayBitmap(const uint8_t* bitmap_buff, size_t bitmap_size) = 0;
    virtual void ClearDisplay() = 0;
    virtual void InvertColors() = 0;
    virtual void SetContrast(uint8_t contrast) = 0;
};