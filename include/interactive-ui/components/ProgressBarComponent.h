#pragma once

#include "TextComponent.h"

// WIP class do not use
class ProgressBarComponent : public TextBoxComponent
{
protected:
    size_t decimal_places{0};

public:
    ProgressBarComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& box_dimensions, const Font* font, int32_t z_layer, Screen* initial_screen = nullptr);
    virtual ~ProgressBarComponent();
    
    void SetDecimalPlaces(size_t places);

    inline size_t GetDecimalPlaces() const
    {
        return decimal_places;
    }
};