#pragma once

#include "../SelectableComponent.h"
#include "../TextProperties.h"

class TextComponent : public SelectableComponent
{
protected:
    TextProperties text_properties;
    size_t message_pixel_length;

public:
    TextComponent(Screen* screen, float x_percentage, float y_percentage, const TextProperties& props, int32_t z_layer);
    TextComponent(Screen* screen, const Vec2u32& origin, const TextProperties& props, int32_t z_layer);

    void Draw() override;
    void DrawSelection() override;

    inline const TextProperties& GetTextProperties() const
    {
        return text_properties;
    }

    inline void SetText(const char* text)
    {
        text_properties.text = text;
    }
};

class TextBoxComponent : public TextComponent
{
protected:
    Vec2u32 padding;

public:
    TextBoxComponent(Screen* screen, const Vec2u32& origin, const Vec2u32& dimensions, const Vec2u32& padding, const TextProperties& props, int32_t z_layer);
    TextBoxComponent(Screen* screen, float x_percentage, float y_percentage, const Vec2u32& dimensions, const Vec2u32& padding, const TextProperties& props, int32_t z_layer);
    TextBoxComponent(Screen* screen, const Vec2u32& origin, const Vec2u32& dimensions, float x_pad_percentage, float y_pad_percentage, const TextProperties& props, int32_t z_layer);
    TextBoxComponent(Screen* screen, float x_percentage, float y_percentage, const Vec2u32& dimensions, float x_pad_percentage, float y_pad_percentage, const TextProperties& props, int32_t z_layer);

    void Draw() override;
};