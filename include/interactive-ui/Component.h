#pragma once

#include "Screen.h"
#include <math/Graphics.h>

class Component;

using graphics::easing::EasingFunctionLUT; // maybe not the best idea to throw in a header? eh dont care enough
struct MovementAnimation
{
private:
    float elapsed = 0.f;

public:
    Vec2i32 start_pos;
    Vec2i32 end_pos = {0, 0};
    float duration = 1.f;
    bool moving = true;
    bool reverse = false;

    const EasingFunctionLUT& easing_func;

    MovementAnimation(const Component* component, const EasingFunctionLUT& easing_func);
    MovementAnimation();

    inline Vec2i32 GetDelta() const
    {
        if (reverse)
            return start_pos - end_pos;
        return end_pos - start_pos;
    }

    friend Component;
};

class Component
{
private:
    static constexpr size_t moving_queue_size = 4;

protected:
    Vec2i32 origin_position;
    Vec2i32 draw_dimensions;
    int32_t z_layer;
    uint32_t color;

    DisplayInterface* display;
    const ScreenManager* manager;

    bool forced_visibility; // master visibility, controlled by screens and managers
    bool personal_visibility; // personal visibility, for hiding and showing on its own

    mutable queue_t moving_queue; // queue containing animation objects. NOT pointers, but values

public:
    const bool selectable; // Smallest overhead without enabling RTTI.

    Component(const ScreenManager* manager, const Vec2i32& position, int32_t z_layer, const Screen* initial_screen = nullptr, bool selectable = false);
    Component(const ScreenManager* manager, float x_percentage, float y_percentage, int32_t z_layer, const Screen* initial_screen, bool selectable = false);
    virtual ~Component();

    virtual void Update(float dt);

    virtual void Draw() = 0;

    bool Move(const MovementAnimation* animation);

    bool IsMoving() const;

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
    inline bool GetForcedVisibility() const
    {
        return forced_visibility;
    }
    inline bool GetPersonalVisibility() const
    {
        return personal_visibility;
    }
    inline bool IsVisible() const
    {
        return forced_visibility && personal_visibility;
    }
    inline Vec2i32 GetOriginPosition() const
    {
        return origin_position;
    }
    inline Vec2i32 GetDrawDimensions() const
    {
        return draw_dimensions;
    }
    inline int32_t GetZLayer() const
    {
        return z_layer;
    }

    friend Screen;
};