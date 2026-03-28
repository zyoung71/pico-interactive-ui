#pragma once

#include <math/AABB.h>

#include "graphics/Font.h"
#include "graphics/Color.h"

using graphics::RGBA;
using graphics::HSV;

enum TextScrollRepitition
{
    ONCE,
    ENDLESS,
    REVERSING
};

class ScreenManager;

class DisplayInterface
{
public:
    virtual ~DisplayInterface() = default;

    virtual Vec2u32 GetDimensions() const = 0;

    virtual void UpdateDisplay() = 0;
    virtual void Power(bool power_on) = 0;
    virtual void SetBrightness(uint8_t contrast) = 0;
    virtual void ClearDisplay() = 0;
    virtual void InvertColors() = 0;

    virtual void DrawPixel(Vec2i32 pos, RGBA color) = 0;
    
    void DrawCharacter(Vec2i32 pos, char c, const Font& font, uint32_t scale, RGBA color);
    void DrawText(Vec2i32 pos, const char* text, const Font& font, uint32_t scale, RGBA color);
    void DrawHorizontalLine(Vec2i32 pos_begin, int32_t length, RGBA color);
    void DrawVerticalLine(Vec2i32 pos_begin, int32_t length, RGBA color);
    void DrawLine(Vec2i32 pos_begin, Vec2i32 pos_end, RGBA color);
    void DrawPolygon(const Vec2i32* points, size_t pos_count, RGBA color);
    void DrawPolygon(const ArrayView<Vec2i32>& points, RGBA color);
    void DrawCircle(Vec2i32 center_pos, int32_t radius, RGBA color);
    void DrawEllipse(Vec2i32 center_pos, Vec2i32 radius, RGBA color);
    void DrawSquare(Vec2i32 pos, Vec2i32 size, RGBA color);
    void DrawSquare(AABBi32 dimensions, RGBA color);

    void DrawBitmap(const uint8_t* bitmap_buff, size_t bitmap_size);
    void DrawBitmap(const ArrayView<uint8_t>& bitmap);

    void FillPolygon(const Vec2i32* points, size_t pos_count, RGBA color);
    void FillPolygon(const ArrayView<Vec2i32>& points, RGBA color);
    void FillCircle(Vec2i32 center_pos, int32_t radius, RGBA color);
    void FillEllipse(Vec2i32 center_pos, Vec2i32 radius, RGBA color);
    void FillSquare(Vec2i32 pos, Vec2i32 size, RGBA color);
    void FillSquare(AABBi32 dimensions, RGBA color);

};