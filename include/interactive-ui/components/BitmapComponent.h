#pragma once

#include "../SelectableComponent.h"

#include <util/ArrayView.h>

class BitmapComponent : public SelectableComponent
{
protected:
    ArrayView2D<uint32_t> pixel_map;

    bool mirror_vertically = false;
    bool mirror_horizontally = false;

public:
    BitmapComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& dimensions, uint32_t* pixels, int32_t z_layer, Screen* initial_screen = nullptr);
    BitmapComponent(ScreenManager* manager, float x_percentage, float y_percentage, const Vec2i32& dimensions, uint32_t* pixels, int32_t z_layer, Screen* initial_screen);
    BitmapComponent(ScreenManager* manager, const Vec2i32& origin, const ArrayView2D<uint32_t>& pixel_map, int32_t z_layer, Screen* initial_screen = nullptr);
    BitmapComponent(ScreenManager* manager, float x_percentage, float y_percentage, const ArrayView2D<uint32_t>& pixel_map, int32_t z_layer, Screen* initial_screen);

    inline void MirrorVertically(bool _override = true)
    {
        mirror_vertically = _override;
    }
    inline void MirrorHorizontally(bool _override = true)
    {
        mirror_horizontally = _override;
    }

    virtual void Draw() override;
};