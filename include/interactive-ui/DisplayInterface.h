#pragma once

#include <math/Vec2.h>
#include "TextProperties.h"

class DisplayInterface
{
public:
    virtual ~DisplayInterface() = default;

    virtual void UpdateDisplay() = 0;
    virtual void Power(bool power_on) = 0;
    
    virtual void DrawText(Vec2u32 pos, const TextProperties& props, uint32_t color) = 0;
    virtual void DrawPixel(Vec2u32 pos, uint32_t color) = 0;
    virtual void DrawLine(Vec2u32 pos_begin, Vec2u32 pos_end, uint32_t color) = 0;
    virtual void DrawPolygon(const Vec2u32* points, size_t pos_count, uint32_t color, bool is_outline = true) = 0;
    virtual void DrawSquare(Vec2u32 pos, Vec2u32 size, uint32_t color, bool is_outline = true) = 0;
    
    virtual void ChangeFont(void* font_data) = 0; // i am not creating a font standard, each device will handle it

    virtual void DisplayBitmap(const uint8_t* bitmap_buff, size_t bitmap_size) = 0;
    virtual void ClearDisplay() = 0;
    virtual void InvertColors() = 0;
    virtual void SetContrast(uint8_t contrast) = 0;
};