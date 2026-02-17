#pragma once

#include "../Component.h"
#include "../iface/IThickness.h"

// Design breaks and separators.
class PaddingComponent : public Component, public IThickness<uint32_t>
{
protected:
    uint32_t thickness = 1;
    
public:
    bool outlined = true;
    bool fill_if_outlined = false;

public:
    PaddingComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& size, int32_t z_layer, Screen* initial_screen = nullptr);
    PaddingComponent(ScreenManager* manager, const Vec2f& screen_percentage, const Vec2i32& size, int32_t z_layer, Screen* initial_screen);

    void Draw() override;

    inline uint32_t GetThickness() const override
    {
        return thickness;
    }
    inline void SetThickness(uint32_t thick) override
    {
        thickness = thick;
    }
    inline Component* GetComponent() const override
    {
        return (Component*)this;
    }
};