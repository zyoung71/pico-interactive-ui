#pragma once

#include "../SelectableComponent.h"

class TextComponent : public SelectableComponent
{
protected:
    const char* text;
    const Font* font;
    size_t lines;
    size_t message_pixel_length;

public:
    TextComponent(const Screen* screen, float x_percentage, float y_percentage, const char* text, const Font* font, int32_t z_layer);
    TextComponent(const Screen* screen, const Vec2u32& origin, const char* text, const Font* font, int32_t z_layer);

    void Draw() override;

    inline const char* GetText() const
    {
        return text;
    }
    inline void SetText(const char* text)
    {
        this->text = text;
    }

    inline const Font* GetFont() const
    {
        return font;
    }
    inline void SetFont(const Font* font)
    {
        this->font = font;
    }

    inline size_t GetTextLines() const
    {
        return lines;
    }
};

class TextBoxComponent : public TextComponent
{
protected:
    Vec2u32 padding;

public:
    TextBoxComponent(const Screen* screen, const Vec2u32& origin, const Vec2u32& dimensions, const Vec2u32& padding, const char* text, const Font* font, int32_t z_layer);
    TextBoxComponent(const Screen* screen, float x_percentage, float y_percentage, const Vec2u32& dimensions, const Vec2u32& padding, const char* text, const Font* font, int32_t z_layer);
    TextBoxComponent(const Screen* screen, const Vec2u32& origin, const Vec2u32& dimensions, float x_pad_percentage, float y_pad_percentage, const char* text, const Font* font, int32_t z_layer);
    TextBoxComponent(const Screen* screen, float x_percentage, float y_percentage, const Vec2u32& dimensions, float x_pad_percentage, float y_pad_percentage, const char* text, const Font* font, int32_t z_layer);

    void Draw() override;
};