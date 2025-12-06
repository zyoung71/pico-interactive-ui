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

    bool forced_visibility;
    bool personal_visibility;

public:
    const bool selectable; // Smallest overhead without enabling RTTI.

    Component(Screen* screen, const Vec2u32& position, int32_t z_layer, bool selectable = false);
    Component(Screen* screen, float x_percentage, float y_percentage, int32_t z_layer, bool selectable = false);
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
    inline void ForceVisibility(bool visibility)
    {
        forced_visibility = visibility;
    }
    inline void SetPersonalVisibility(bool visibility)
    {
        personal_visibility = visibility;
    }
    inline bool IsVisible() const
    {
        return forced_visibility && personal_visibility;
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