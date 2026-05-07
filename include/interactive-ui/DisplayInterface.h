#pragma once

#include <math/AABB.h>

#include "graphics/Font.h"
#include "graphics/Color.h"

using graphics::RGBA;
using graphics::HSV;
namespace colors = graphics::colors;

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
    inline AABBi32 GetDimensionsAABB() const
    {
        return AABBi32{0, 0, static_cast<int32_t>(GetDimensions().x), static_cast<int32_t>(GetDimensions().y)};
    }

    virtual void UpdateDisplay() = 0;
    virtual void Power(bool power_on) = 0;
    virtual void SetBrightness(uint8_t contrast) = 0;
    virtual void ClearDisplay() = 0;
    virtual void InvertColors() = 0;

    virtual void DrawPixel(Vec2i32 pos, RGBA color) = 0;
    virtual void DrawPixel(int32_t x, int32_t y, RGBA color) = 0;
    
    void DrawCharacter(Vec2i32 pos, char c, const Font& font, uint32_t scale, RGBA color);
    void DrawText(Vec2i32 pos, const char* text, const Font& font, uint32_t scale, RGBA color);

    void DrawHorizontalLine(Vec2i32 pos_begin, int32_t length, RGBA color);
    void DrawHorizontalLineX1X2(int32_t x1, int32_t x2, int32_t y, RGBA color);
    void DrawVerticalLine(Vec2i32 pos_begin, int32_t length, RGBA color);
    void DrawVerticalLineY1Y2(int32_t y1, int32_t y2, int32_t x, RGBA color);
    void DrawLine(Vec2i32 pos_begin, Vec2i32 pos_end, RGBA color);

    void DrawPolygon(const Vec2i32* points, size_t pos_count, RGBA color);
    void DrawPolygon(const ArrayView<Vec2i32>& points, RGBA color);

    void DrawCircle(Vec2i32 center_pos, int32_t radius, RGBA color);
    void DrawEllipse(Vec2i32 center_pos, Vec2i32 radius, RGBA color);

    void DrawRectangle(Vec2i32 pos, Vec2i32 size, RGBA color);
    void DrawRectangle(AABBi32 dimensions, RGBA color);

    void DrawRoundedRectangle(Vec2i32 pos, Vec2i32 size, Vec2i32 radius, RGBA color);
    void DrawRoundedRectangle(AABBi32 dimensions, Vec2i32 radius, RGBA color);

    void FillPolygon(const Vec2i32* points, size_t pos_count, RGBA color);
    void FillPolygon(const ArrayView<Vec2i32>& points, RGBA color);

    void FillCircle(Vec2i32 center_pos, int32_t radius, RGBA color);
    void FillEllipse(Vec2i32 center_pos, Vec2i32 radius, RGBA color);

    void FillRectangle(Vec2i32 pos, Vec2i32 size, RGBA color);
    void FillRectangle(AABBi32 dimensions, RGBA color);
    void FillRectangle(int32_t x_0, int32_t y_0, int32_t len_x, int32_t len_y, RGBA color);

};