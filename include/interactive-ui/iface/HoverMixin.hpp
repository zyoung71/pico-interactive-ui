#pragma once

#include "IComponentSupplier.hpp"
#include <math/Vec2.hpp>
#include <math/AABB.hpp>

class HoverMixin : public IComponentSupplier
{
public:
    static constexpr Vec2i32 hover_initial_position{-1, -1};
    static constexpr int32_t hover_z_layer = 5000;

    Component* hovered_component;

public:
    virtual void OnLock() = 0;
    virtual void OnUnlock() = 0;

    virtual AABBi32 GetDrawOffset() const = 0;
};