#pragma once

#include "../SelectableComponent.h"

class TextComponent : public SelectableComponent
{
protected:
    const char* text;
    const Font* font;
    Vec2i32 message_pixel_dimensions;
    Vec2i32 text_base_offset; // the offset from the min of the draw dimensions

    virtual void UpdateTextDimensions();
    void DrawTextWithAlignment(AlignmentHorizontal align_h);

public:
    TextComponent(ScreenManager* manager, const Vec2i32& origin, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen = nullptr);
    TextComponent(ScreenManager* manager, float x_percentage, float y_percentage, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen);

    virtual void Draw() override;
    virtual void Align() override;

    void SetText(const char* text);

    inline const char* GetText() const
    {
        return text;
    }
    inline const Font* GetFont() const
    {
        return font;
    }
    inline void SetFont(const Font* font)
    {
        this->font = font;
    }
};

class TextBoxComponent : public TextComponent
{
protected:
    Vec2i32 padding;
    AlignmentVertical text_vertical_alignment;
    AlignmentHorizontal text_horizontal_alignment;

    virtual void UpdateTextDimensions() override;

public:
    TextBoxComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& box_dimensions, const Vec2i32& padding, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen = nullptr);
    TextBoxComponent(ScreenManager* manager, float x_percentage, float y_percentage, const Vec2i32& box_dimensions, const Vec2i32& padding, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen);
    TextBoxComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& box_dimensions, float x_pad_percentage, float y_pad_percentage, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen = nullptr);
    TextBoxComponent(ScreenManager* manager, float x_percentage, float y_percentage, const Vec2i32& box_dimensions, float x_pad_percentage, float y_pad_percentage, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen);

    virtual void Draw() override;

    void SetTextVerticalAlignment(AlignmentVertical align_v);
    void SetTextHorizontalAlignment(AlignmentHorizontal align_h);

    inline AlignmentVertical GetTextVerticalAlignment() const
    {
        return text_vertical_alignment;
    }
    inline AlignmentHorizontal GetTextHorizontalAlignment() const
    {
        return text_horizontal_alignment;
    }
};