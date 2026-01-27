#pragma once

#include "Screen.h"
#include <math/Graphics.h>

class Component;

struct MovementAnimation
{
private:
    float elapsed = 0.f;

public:
    Vec2u32 start_pos;
    Vec2u32 end_pos = {0, 0};
    float duration = 1.f;
    bool moving = true;

    typedef std::array<float, graphics::easing::lut_size> EasingFunctionLUT;

    const EasingFunctionLUT& easing_func;

    MovementAnimation(const Component* component, const EasingFunctionLUT& easing_func);
    MovementAnimation();

    inline Vec2u32 GetDelta() const
    {
        return end_pos - start_pos;
    }

    friend Component;
};

class Component
{
private:
    static constexpr size_t moving_queue_size = 4;

protected:
    Vec2u32 origin_position;
    Vec2u32 draw_dimensions;
    int32_t z_layer;
    uint32_t color;

    DisplayInterface* display;
    const ScreenManager* manager;

    bool forced_visibility; // master visibility, controlled by screens and managers
    bool personal_visibility; // personal visibility, for hiding and showing on its own

    mutable queue_t moving_queue; // queue containing animation objects. NOT pointers, but values

public:
    const bool selectable; // Smallest overhead without enabling RTTI.

    Component(const ScreenManager* manager, const Vec2u32& position, int32_t z_layer, const Screen* initial_screen = nullptr, bool selectable = false);
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

    friend Screen;
};