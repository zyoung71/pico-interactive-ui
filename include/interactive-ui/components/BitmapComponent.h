#pragma once

#include "../SelectableComponent.h"
#include "../iface/IScalable.h"

#include <util/ArrayView.h>

class BitmapComponent : public SelectableComponent, public IScalable<uint32_t>
{
public:
    ArrayView2D<RGBA> pixel_map;
    uint32_t scale = 1;
    bool mirror_vertically = false;
    bool mirror_horizontally = false;
    bool override_color = false;

public:
    BitmapComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& dimensions, RGBA* pixels, int32_t z_layer, Screen* initial_screen = nullptr);
    BitmapComponent(ScreenManager* manager, const Vec2f& screen_percentage, const Vec2i32& dimensions, RGBA* pixels, int32_t z_layer, Screen* initial_screen);
    BitmapComponent(ScreenManager* manager, const Vec2i32& origin, const ArrayView2D<RGBA>& pixel_map, int32_t z_layer, Screen* initial_screen = nullptr);
    BitmapComponent(ScreenManager* manager, const Vec2f& screen_percentage, const ArrayView2D<RGBA>& pixel_map, int32_t z_layer, Screen* initial_screen);
    virtual ~BitmapComponent() = default;

    virtual void Draw(const Screen* screen) override;

    inline void SetScale(uint32_t s) override
    {
        scale = s;
    }
    inline uint32_t GetScale() const override
    {
        return scale;
    }
};