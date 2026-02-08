#pragma once

#include "../Component.h"

// Design breaks and separators.
class PaddingComponent : public Component
{
protected:
    bool outlined;

public:
    PaddingComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& size, bool outline, int32_t z_layer, Screen* initial_screen = nullptr);
    PaddingComponent(ScreenManager* manager, const Vec2f& screen_percentage, const Vec2i32& size, bool outline, int32_t z_layer, Screen* initial_screen);

    void Draw() override;

};