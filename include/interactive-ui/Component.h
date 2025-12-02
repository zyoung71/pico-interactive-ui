#pragma once

#include "Screen.h"

class Component
{
protected:
    Vec2u32 origin_position;
    Vec2u32 draw_dimensions;

    ScreenData data;

    bool is_visible;

public:
    Component(Screen* screen, const Vec2u32& position);
    Component(Screen* screen, float x_percentage, float y_percentage);
    virtual ~Component();

    virtual void Update(float dt);

    virtual void Draw() = 0;

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
};