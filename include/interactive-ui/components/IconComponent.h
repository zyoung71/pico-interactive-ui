#pragma once

#include "../SelectableComponent.h"

#include <util/ArrayView.h>

class IconComponent : public SelectableComponent
{
protected:
    ArrayView2D<uint32_t> pixel_map;

public:
    IconComponent(const Screen* screen, const Vec2u32& origin, const Vec2u32& dimensions, uint32_t* pixels, int32_t z_layer);
    IconComponent(const Screen* screen, float x_percentage, float y_percentage, const Vec2u32& dimensions, uint32_t* pixels, int32_t z_layer);
    IconComponent(const Screen* screen, const Vec2u32& origin, const ArrayView2D<uint32_t>& pixel_map, int32_t z_layer);
    IconComponent(const Screen* screen, float x_percentage, float y_percentage, const ArrayView2D<uint32_t>& pixel_map, int32_t z_layer);

    virtual void Draw() override;
};