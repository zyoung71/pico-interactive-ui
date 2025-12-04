#pragma once

#include "Screen.h"

class Component
{
protected:
    Vec2u32 origin_position;
    Vec2u32 draw_dimensions;
    int32_t z_layer;
    uint32_t color;

    ScreenData data;

    bool is_visible;

public:
    Component(Screen* screen, const Vec2u32& position, int32_t z_layer);
    Component(Screen* screen, float x_percentage, float y_percentage, int32_t z_layer);
    virtual ~Component() = default;

    virtual void Update(float dt);

    virtual void Draw() = 0;

    inline void SetColor(uint32_t rgba)
    {
        color = rgba;
    }
    inline uint32_t GetColor() const
    {
        return color;
    }
    inline void SetVisible(bool is_visible)
    {
        this->is_visible = is_visible;
    }
    inline bool IsVisible() const
    {
        return is_visible;
    }
    inline Vec2u32 GetOriginPosition() const
    {
        return origin_position;
    }
    inline Vec2u32 GetDrawDimensions() const
    {
        return draw_dimensions;
    }
    inline int32_t GetZLayer() const
    {
        return z_layer;
    }
};