#pragma once

#include <math/Vec2.h>

class DisplayInterface
{
public:
    virtual ~DisplayInterface() = default;

    virtual void DrawText(Vec2u32 pos, const char* str) = 0;
    virtual void DrawLine(Vec2u32 pos_begin, Vec2u32 pos_end) = 0;
    virtual void DrawPolygon(Vec2u32* points, size_t pos_count) = 0;
    virtual void DrawSquare(Vec2u32 pos, Vec2u32 size) = 0;
    virtual void DrawBitmap(Vec2u32 pos, void* bitmap_buff, size_t bitmap_size) = 0;
};