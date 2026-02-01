#include <interactive-ui/components/TextComponent.h>

#include <cstring>

void TextComponent::UpdateTextDimensions()
{
    const size_t msg_len = strlen(text);
    size_t lines = 1;

    size_t segment_max = 0;
    size_t iter = 0;

    while (iter < msg_len) // failsafe
    {
        /**
         * Example:
         * "Hello\n.\nWorld"
         * msg_len = 13
         * first iteration: line_len = 5, 13 - 0 != 5, iter goes from 0 to 6, lines goes from 1 to 2
         * second iteration: line_len = 1, 13 - 6 != 1, iter goes from 6 to 8, lines goes from 2 to 3
         * third iteration: line_len = 5, 13 - 8 == 5, ends
         */
        size_t line_len = strcspn(text + iter, "\n");
        if (line_len > segment_max)
            segment_max = line_len;

        if (msg_len - iter == line_len)
            break;
            
        iter += line_len + 1;
        lines++;
    }
    message_pixel_dimensions.x = static_cast<int32_t>(segment_max * font->char_width + (segment_max - 1) * font->char_spacing);
    message_pixel_dimensions.y = static_cast<int32_t>(font->char_height * lines + font->char_spacing * (lines - 1));
}

TextComponent::TextComponent(ScreenManager* manager, const Vec2i32& origin, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen)
    : SelectableComponent(manager, origin, z_layer, initial_screen), text(text), font(font)
{
    UpdateTextDimensions();
    draw_dimensions.max = message_pixel_dimensions;
}

TextComponent::TextComponent(ScreenManager* manager, float x_percentage, float y_percentage, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen)
    : SelectableComponent(manager, x_percentage, y_percentage, z_layer, initial_screen), text(text), font(font)
{
    UpdateTextDimensions();
    draw_dimensions.max = message_pixel_dimensions;   
}

void TextComponent::Draw()
{
    size_t len = strlen(text) + 1;
    char vbuff[len];
    size_t track = 0;
    size_t line = 0;

    while (track < len)
    {
        /**
         * Example:
         * "Hello\n.\nWorld"
         * len = 14 (13 char + 1)
         * first iteration: segment_len = 5, copy "Hello" info vbuff, span = 5, vbuff[5] = nullterm, draw "Hello", track goes from 0 to 6
         * second iteration: segment_len = 1, copy "." info vbuff + 6, span = 7, vbuff[7] = nullterm, draw ".", track goes from 6 to 8
         * third iteration: segment_len = 5, copy "World" info vbuff + 8, span = 13, vbuff[13] = nullterm, draw "World", track goes from 8 to 13
         * 
         * Example 2:
         * "test\n\ntext"
         * len = 11
         * first iteration: segment_length = 4, copy "test" info vbuff, span = 4, vbuff[4] = nullterm, draw "test", track goes from 0 to 5
         * second iteration: segment_length = 0, copy "" info vbuff, span = 5, vbuff[5] = nullterm, draw "", track goes from 5 to 6
         * third iteration: segment_length = 4, copy "text" info vbuff, span = 10, vbuff[10] = nullterm, draw "text", track goes from 6 to 11
         */
        size_t segment_len = strcspn(text + track, "\n");
        size_t segment_pixel_length = segment_len * font->char_width + (segment_len - 1) * font->char_spacing;
        strncpy(vbuff + track, text + track, segment_len);

        size_t span = segment_len + track;
        vbuff[span] = '\0';

        Vec2i32 text_offset = text_base_offset;
        text_offset.y += static_cast<int32_t>((line++) * (font->char_spacing + font->char_height));
        switch (horizontal_alignment)
        {
            case AlignmentHorizontal::LEFT: break;
            case AlignmentHorizontal::CENTER: {
                text_offset.x += message_pixel_dimensions.x / 2 - segment_pixel_length / 2;
                break;
            }
            case AlignmentHorizontal::RIGHT: {
                text_offset.x += message_pixel_dimensions.x - segment_pixel_length;
                break;
            }
        }

        display->DrawText(origin_position + draw_dimensions.min + text_offset, vbuff + track, font, color);
        track = span + 1;
    }
}

void TextComponent::Align()
{
    Component::Align();
    UpdateTextDimensions();
}

void TextComponent::SetText(const char* text)
{
    this->text = text;
    UpdateTextDimensions();
}

void TextBoxComponent::UpdateTextDimensions()
{
    TextComponent::UpdateTextDimensions();
    switch (vertical_alignment)
    {
        case AlignmentVertical::TOP: {
            text_base_offset.y = padding.y;
            break;
        }
        case AlignmentVertical::CENTER: {
            text_base_offset.y = (draw_dimensions.ymax - draw_dimensions.ymin) / 2 - message_pixel_dimensions.y / 2;
            break;
        }
        case AlignmentVertical::BOTTOM: {
            text_base_offset.y = (draw_dimensions.ymax - draw_dimensions.ymin) - message_pixel_dimensions.y - padding.y;
            break;
        }
    }
    switch (horizontal_alignment)
    {
        case AlignmentHorizontal::LEFT: { 
            text_base_offset.x = padding.x;
            break;
        }
        case AlignmentHorizontal::CENTER: { 
            text_base_offset.x = (draw_dimensions.xmax - draw_dimensions.xmin) / 2 - message_pixel_dimensions.x / 2;
            
            break;
        }
        case AlignmentHorizontal::RIGHT: {
            text_base_offset.x = (draw_dimensions.xmax - draw_dimensions.xmin) - message_pixel_dimensions.x - padding.x;
            break;
        }
    }
}

TextBoxComponent::TextBoxComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& box_dimensions, const Vec2i32& padding, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen)
    : TextComponent(manager, origin, text, font, z_layer, initial_screen), padding(padding)
{
    draw_dimensions.max = box_dimensions;
    UpdateTextDimensions();
}
TextBoxComponent::TextBoxComponent(ScreenManager* manager, float x_percentage, float y_percentage, const Vec2i32& box_dimensions, const Vec2i32& padding, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen)
    : TextComponent(manager, x_percentage, y_percentage, text, font, z_layer, initial_screen), padding(padding)
{
    draw_dimensions.max = box_dimensions;
    UpdateTextDimensions();
}
TextBoxComponent::TextBoxComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& box_dimensions, float x_pad_percentage, float y_pad_percentage, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen)
    : TextComponent(manager, origin, text, font, z_layer, initial_screen), padding({static_cast<int32_t>(box_dimensions.x * x_pad_percentage), static_cast<int32_t>(box_dimensions.y * y_pad_percentage)})
{
    draw_dimensions.max = box_dimensions;
    UpdateTextDimensions();
}
TextBoxComponent::TextBoxComponent(ScreenManager* manager, float x_percentage, float y_percentage, const Vec2i32& box_dimensions, float x_pad_percentage, float y_pad_percentage, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen)
    : TextComponent(manager, x_percentage, y_percentage, text, font, z_layer, initial_screen), padding({static_cast<int32_t>(box_dimensions.x * x_pad_percentage), static_cast<int32_t>(box_dimensions.y * y_pad_percentage)})
{
    draw_dimensions.max = box_dimensions;
    UpdateTextDimensions();
}

void TextBoxComponent::Draw()
{
    TextComponent::Draw();
    display->DrawSquare(origin_position + draw_dimensions.min, draw_dimensions.max - draw_dimensions.min, color, true); // box
}