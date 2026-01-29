#pragma once

#include "../SelectableComponent.h"

#include <util/ArrayView.h>

class BitmapComponent : public SelectableComponent
{
protected:
    ArrayView2D<uint32_t> pixel_map;

public:
    BitmapComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& dimensions, uint32_t* pixels, int32_t z_layer, const Screen* initial_screen = nullptr);
    BitmapComponent(ScreenManager* manager, float x_percentage, float y_percentage, const Vec2i32& dimensions, uint32_t* pixels, int32_t z_layer, const Screen* initial_screen);
    BitmapComponent(ScreenManager* manager, const Vec2i32& origin, const ArrayView2D<uint32_t>& pixel_map, int32_t z_layer, const Screen* initial_screen = nullptr);
    BitmapComponent(ScreenManager* manager, float x_percentage, float y_percentage, const ArrayView2D<uint32_t>& pixel_map, int32_t z_layer, const Screen* initial_screen);

    virtual void Draw() override;
};