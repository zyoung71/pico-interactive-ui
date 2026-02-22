#pragma once

#include "../Component.h"
#include "../iface/IThickness.h"

class CircleComponent : public Component, public IThickness<uint32_t>
{
protected:
    uint32_t radius;
    uint32_t thickness = 1;

public:
    CircleComponent(ScreenManager* manager, const Vec2i32& origin, uint32_t radius, int32_t z_layer, Screen* initial_screen = nullptr);
    CircleComponent(ScreenManager* manager, const Vec2f& percentage, uint32_t radius, int32_t z_layer, Screen* initial_screen);

    void Draw() override;

    uint32_t GetThickness() const override
    {
        return thickness;
    }
    inline void SetThickness(uint32_t thickness) override
    {
        this->thickness = thickness;
    }
    inline Component* GetComponent() const override
    {
        return (Component*)this;
    }

};