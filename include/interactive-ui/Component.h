#pragma once

#include "Screen.h"
#include <math/Graphics.h>
#include <math/AABB.h>

class Component;

using graphics::easing::EasingFunctionLUT; // maybe not the best idea to throw in a header? eh dont care enough
struct MovementAnimation
{
private:
    float elapsed = 0.f;
    bool reversed = false;
    bool enable_callbacks = true;
    bool moving = true;

public:
    enum Type
    {
        NORMAL,
        ENDLESS,
        ENDLESS_WITH_REVERSION
    };

    AABBi32 start_scale;
    AABBi32 end_scale;
    Vec2i32 start_pos;
    Vec2i32 end_pos;
    float duration = 1.f;
    Type type = NORMAL;
    bool transpose = true;
    bool scale = false;

    const EasingFunctionLUT& easing_func;

    typedef void(*AnimationStateCallback)(const MovementAnimation*);

    AnimationStateCallback on_animation_begin = nullptr;
    AnimationStateCallback on_animation_end = nullptr;

    MovementAnimation(const Component* component, const EasingFunctionLUT& easing_func);
    MovementAnimation();

    inline Vec2i32 GetTransposeDelta() const
    {
        return end_pos - start_pos;
    }
    inline Vec4i32 GetScaleDelta() const
    {
        return end_scale.vec - start_scale.vec;
    }

    friend Component;
};

enum class AlignmentVertical
{
    TOP,
    CENTER,
    BOTTOM
};

enum class AlignmentHorizontal
{
    LEFT,
    CENTER,
    RIGHT
};

class Component
{
private:
    static constexpr size_t moving_queue_size = 4;

    void MoveRefInc();
    void MoveRefDec();
    
private:
    bool cancel_movements_flag;
    mutable queue_t moving_queue; // queue containing animation objects. NOT pointers, but values

protected:
    AlignmentVertical vertical_alignment = AlignmentVertical::TOP;
    AlignmentHorizontal horizontal_alignment = AlignmentHorizontal::LEFT;

    DisplayInterface* display;
    ScreenManager* manager;

    std::unordered_set<Screen*> screen_set;

    bool forced_visibility; // master visibility, controlled by screens and managers
    bool personal_visibility; // personal visibility, for hiding and showing on its own
    
    AABBi32 draw_dimensions; // most of the time this will be treated as a single vector, specifically the max
    Vec2i32 origin_position;
    int32_t z_layer;

public:
    uint32_t color;

public:
    const bool selectable; // Smallest overhead without enabling RTTI.

    Component(ScreenManager* manager, const Vec2i32& position, int32_t z_layer, Screen* initial_screen = nullptr, bool selectable = false);
    Component(ScreenManager* manager, const Vec2f& screen_percentage, int32_t z_layer, Screen* initial_screen, bool selectable = false);
    Component(const Component& to_copy);
    virtual ~Component();

    virtual void Update(float dt, const Screen* screen);
    virtual void Draw() = 0;

    virtual void OnEnterScreen(const Screen* screen) {}
    virtual void OnExitScreen(const Screen* screen) {}

    virtual void Align();
    virtual void Scale() {} // Scaling support is only available to classes that implement it

    virtual void SetOriginPosition(const Vec2i32& pos);

    void SetVerticalAlignment(AlignmentVertical align_v);
    void SetHorizontalAlignment(AlignmentHorizontal align_h);
    void SetAlignment(AlignmentVertical align_v, AlignmentHorizontal align_h);
    void SetZLayer(int32_t z_layer);

    bool Move(MovementAnimation animation, bool reversed = false, bool enable_callbacks = true);
    bool IsMoving() const;
    inline void CancelMovementsNextUpdate() // needed to stop endless loops
    {
        cancel_movements_flag = true;
    }
    virtual inline void ForceVisibility(bool visibility)
    {
        forced_visibility = visibility;
    }
    virtual inline void SetPersonalVisibility(bool visibility)
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
    inline int32_t GetZLayer() const
    {
        return z_layer;
    }
    inline AlignmentVertical GetVerticalAlignment() const
    {
        return vertical_alignment;
    }
    inline AlignmentHorizontal GetHorizontalAlignment() const
    {
        return horizontal_alignment;
    }
    inline AABBi32 GetDrawDimensions() const
    {
        return draw_dimensions;
    }
    inline Vec2i32 GetOriginPosition() const
    {
        return origin_position;
    }

    friend Screen;
    friend MovementAnimation;
};