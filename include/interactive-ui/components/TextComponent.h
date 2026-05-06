#pragma once

#include "../SelectableComponent.h"

class TextComponent : public SelectableComponent
{
protected:
    Vec2i32 message_pixel_dimensions;
    Vec2i32 text_base_offset; // the offset from the min of the draw dimensions
    const char* text;
    uint32_t font_scale = 1;
    AlignmentVertical text_vertical_alignment;
    AlignmentHorizontal text_horizontal_alignment;
    
    virtual void UpdateTextDimensions();

public:
    const Font* font;

public:
    TextComponent(ScreenManager* manager, const Vec2i32& origin, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen = nullptr);
    TextComponent(ScreenManager* manager, const Vec2f& screen_percentage, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen);
    virtual ~TextComponent() = default;
    
    virtual void Draw(const Screen* screen) override;
    virtual void Align() override;

    void SetText(const char* text);
    void SetFontScale(uint32_t scale);

    /**
     * NOTE: These functions are aimed to align text dependent on draw dimenions.
     * Regular, unmodified TextComponents do not adjust the draw dimensions outside
     * the actual dimensions of the text. If you are looking to adjust around the
     * origin position, please use the SetAlignment family of functions.
     */
    void SetTextVerticalAlignment(AlignmentVertical align_v);
    void SetTextHorizontalAlignment(AlignmentHorizontal align_h);
    void SetTextAlignment(AlignmentVertical align_v, AlignmentHorizontal align_h);
    void SetTextAlignment(AlignmentHorizontal align_h, AlignmentVertical align_v);

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
    inline uint32_t GetFontScale() const
    {
        return font_scale;
    }
    inline Vec2i32 GetMessagePixelDimensions() const
    {
        return message_pixel_dimensions;
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
    virtual ~TextBoxComponent() = default;

    virtual void Draw(const Screen* screen) override;
    virtual void Align() override;

    void SetPadding(const Vec2i32& padding);

    inline Vec2i32 GetPadding() const
    {
        return padding;
    }

    void EnableDynamicBoxDimensions(bool enable);
};