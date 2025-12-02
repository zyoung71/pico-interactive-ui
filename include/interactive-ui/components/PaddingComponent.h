#pragma once

#include "../Component.h"

// Design breaks and separators.
class PaddingComponent : public Component
{
protected:
    bool outlined;

public:
    PaddingComponent(Screen* screen, const Vec2u32& origin, const Vec2u32& size, bool outline, int32_t z_layer);
    PaddingComponent(Screen* screen, float x_percentage, float y_percentage, const Vec2u32& size, bool outline, int32_t z_layer);

    void Draw() override;

};