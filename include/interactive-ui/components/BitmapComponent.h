#pragma once

#include "../SelectableComponent.h"

#include <util/ArrayView.h>

class BitmapComponent : public SelectableComponent
{
public:
    ArrayView2D<uint32_t> pixel_map;
    bool mirror_vertically = false;
    bool mirror_horizontally = false;

public:
    BitmapComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& dimensions, uint32_t* pixels, int32_t z_layer, Screen* initial_screen = nullptr);
    BitmapComponent(ScreenManager* manager, const Vec2f& screen_percentage, const Vec2i32& dimensions, uint32_t* pixels, int32_t z_layer, Screen* initial_screen);
    BitmapComponent(ScreenManager* manager, const Vec2i32& origin, const ArrayView2D<uint32_t>& pixel_map, int32_t z_layer, Screen* initial_screen = nullptr);
    BitmapComponent(ScreenManager* manager, const Vec2f& screen_percentage, const ArrayView2D<uint32_t>& pixel_map, int32_t z_layer, Screen* initial_screen);

    virtual void Draw() override;
};