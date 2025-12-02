#pragma once

#include "../SelectableComponent.h"

#include <memory>

class IconComponent : public SelectableComponent
{
protected:
    std::unique_ptr<uint32_t[]> bitmap;

public:
    IconComponent(Screen* screen, const Vec2u32& origin, const Vec2u32& dimensions, uint32_t* bmp, int32_t z_layer);
    IconComponent(Screen* screen, float x_percentage, float y_percentage, const Vec2u32& dimensions, uint32_t* bmp, int32_t z_layer);

    void Draw() override;
};