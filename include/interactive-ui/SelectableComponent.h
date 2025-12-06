#pragma once

#include "Component.h"

class SelectableComponent : public Component
{
protected:
    bool is_hovered;

public:
    union
    {
        SelectableComponent* neighboring_components[4];
        struct
        {
            SelectableComponent* up_component;
            SelectableComponent* down_component;
            SelectableComponent* left_component;
            SelectableComponent* right_component;
        };
    };

public:
    SelectableComponent(Screen* screen, const Vec2u32& position, int32_t z_layer);
    SelectableComponent(Screen* screen, float x_percentage, float y_percentage, int32_t z_layer);
    virtual ~SelectableComponent() = default;

    inline bool IsHovered() const
    {
        return is_hovered;
    }

    virtual void Update(float dt) override;
    
    virtual void DrawHover(); // Draw the extra elements when the component is hovered. Default is an outlining square.
    virtual void OnComponentHovered() {};
    virtual void OnComponentUnhovered() {};

    friend Screen;
};