#pragma once

#include "../SelectableComponent.h"

class TextComponent : public SelectableComponent
{
protected:
    Vec2i32 message_pixel_dimensions;
    Vec2i32 text_base_offset; // the offset from the min of the draw dimensions
    const char* text;
    AlignmentVertical text_vertical_alignment;
    AlignmentHorizontal text_horizontal_alignment;
    
    virtual void UpdateTextDimensions();

public:
    const Font* font;

public:
    TextComponent(ScreenManager* manager, const Vec2i32& origin, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen = nullptr);
    TextComponent(ScreenManager* manager, const Vec2f& screen_percentage, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen);

    virtual void Draw() override;
    virtual void Align() override;

    void SetText(const char* text);
    void SetTextVerticalAlignment(AlignmentVertical align_v);
    void SetTextHorizontalAlignment(AlignmentHorizontal align_h);
    void SetTextAlignment(AlignmentVertical align_v, AlignmentHorizontal align_h);

    inline AlignmentVertical GetTextVerticalAlignment() const
    {
        return text_vertical_alignment;
    }
    inline AlignmentHorizontal GetTextHorizontalAlignment() const
    {
        return text_horizontal_alignment;
    }
    inline const char* GetText() const
    {
        return text;
    }
};

class TextBoxComponent : public TextComponent
{
protected:
    bool dynamic_box = false;
    Vec2i32 padding = Vec2i32{2, 2};

    virtual void UpdateTextDimensions() override;

public:
    bool clear_bg = false;

public:
    TextBoxComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& box_dimensions, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen = nullptr);
    TextBoxComponent(ScreenManager* manager, const Vec2f& screen_percentage, const Vec2i32& box_dimensions, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen);

    virtual void Draw() override;
    virtual void Align() override;

    void SetPadding(const Vec2i32& padding);

    inline Vec2i32 GetPadding() const
    {
        return padding;
    }

    void EnableDynamicBoxDimensions(bool enable);
};