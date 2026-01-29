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
    TextComponent(ScreenManager* manager, const Vec2i32& origin, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen = nullptr);
    TextComponent(ScreenManager* manager, float x_percentage, float y_percentage, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen);

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
    Vec2i32 padding;

public:
    TextBoxComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& dimensions, const Vec2i32& padding, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen = nullptr);
    TextBoxComponent(ScreenManager* manager, float x_percentage, float y_percentage, const Vec2i32& dimensions, const Vec2i32& padding, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen);
    TextBoxComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& dimensions, float x_pad_percentage, float y_pad_percentage, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen = nullptr);
    TextBoxComponent(ScreenManager* manager, float x_percentage, float y_percentage, const Vec2i32& dimensions, float x_pad_percentage, float y_pad_percentage, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen);

    void Draw() override;
};