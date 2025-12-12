#pragma once

#include "../Component.h"

// Design breaks and separators.
class PaddingComponent : public Component
{
protected:
    bool outlined;

public:
    PaddingComponent(const ScreenManager* manager, const Vec2u32& origin, const Vec2u32& size, bool outline, int32_t z_layer, const Screen* initial_screen = nullptr);
    PaddingComponent(const ScreenManager* manager, float x_percentage, float y_percentage, const Vec2u32& size, bool outline, int32_t z_layer, const Screen* initial_screen);

    void Draw() override;

};