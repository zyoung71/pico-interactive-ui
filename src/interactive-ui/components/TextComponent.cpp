#include <interactive-ui/components/TextComponent.hpp>
#include <util/Platform.h>
#include <util/Chars.h>

#include <cstring>

void TextComponent::UpdateTextDimensions()
{
    if (!font_group)
    {
        LOG("font_group is null!\n");
        return;
    }

    size_t line_width_max = 0;
    size_t line_height_max = 0;
    message_pixel_dimensions.x = 0;
    message_pixel_dimensions.y = 0;

    const Font* selected_font = &fonts::default_font;
    const char* str_iter = text;
    while (*str_iter) // iteration over each character
    {
        size_t chars_used;
        char32_t codepoint = utf8_decode(str_iter, &chars_used);
        str_iter += chars_used;

        if (codepoint == '\n' || !*str_iter) // if new line or last character in string
        {
            message_pixel_dimensions.y += line_height_max;
            line_width_max = 0;
            line_height_max = 0;
            continue;
        }

        for (const Font& f : *font_group) // find font of codepoint
        {
            // if in range
            if (codepoint >= f.character_begin && codepoint <= f.character_end)
            {
                selected_font = &f;
                line_width_max += f.char_width + f.char_spacing_x;

                if ((f.char_height + f.char_spacing_y) > line_height_max)
                    line_height_max = f.char_height + f.char_spacing_y;

                if (line_width_max > message_pixel_dimensions.x)
                    message_pixel_dimensions.x = line_width_max;

                break;
            }
        }
        
    }
    message_pixel_dimensions -= (Vec2i32)selected_font->char_spacing; // subtract extra spacing at end
    message_pixel_dimensions.x *= font_scale.x;
    message_pixel_dimensions.y *= font_scale.y;

    for (int i = 0; i < 100; i++)
    printf("dims: <%i, %i>\n", message_pixel_dimensions.x, message_pixel_dimensions.y);

/* old algorithm using one font */

    // const size_t msg_len = strlen(text);
    // size_t lines = 1;

    // size_t segment_max = 0;
    // size_t iter = 0;

    // while (iter < msg_len) // failsafe
    // {
    //     /**
    //      * Example:
    //      * "Hello\n.\nWorld"
    //      * msg_len = 13
    //      * first iteration: line_len = 5, 13 - 0 != 5, iter goes from 0 to 6, lines goes from 1 to 2
    //      * second iteration: line_len = 1, 13 - 6 != 1, iter goes from 6 to 8, lines goes from 2 to 3
    //      * third iteration: line_len = 5, 13 - 8 == 5, ends
    //      */
    //     size_t line_len = strcspn(text + iter, "\n");
    //     if (line_len > segment_max)
    //         segment_max = line_len;

    //     if (msg_len - iter == line_len)
    //         break;
            
    //     iter += line_len + 1;
    //     lines++;
    // }
    // message_pixel_dimensions.x = font_scale.x * static_cast<int32_t>(segment_max * font->char_width + (segment_max - 1) * font->char_spacing);
    // message_pixel_dimensions.y = font_scale.y * static_cast<int32_t>(font->char_height * lines + font->char_spacing * (lines - 1));

}

TextComponent::TextComponent(ScreenManager* manager, const Vec2i32& origin, const char* text, const FontGroup* font_group, int32_t z_layer, Screen* initial_screen)
    : SelectableComponent(manager, origin, z_layer, initial_screen), text(text), font_group(font_group)
{
    UpdateTextDimensions();
    draw_dimensions.max = message_pixel_dimensions;
}

TextComponent::TextComponent(ScreenManager* manager, const Vec2f& screen_percentage, const char* text, const FontGroup* font_group, int32_t z_layer, Screen* initial_screen)
    : SelectableComponent(manager, screen_percentage, z_layer, initial_screen), text(text), font_group(font_group)
{
    UpdateTextDimensions();
    draw_dimensions.max = message_pixel_dimensions;   
}

void TextComponent::Draw(const Screen* screen)
{
    const Font* selected_font = &fonts::default_font;
    const char* str_iter = text;

    const size_t len = strlen(text) + 1;
    char vbuff[len];
    vbuff[len - 1] = '\0'; // failsafe

    size_t track = 0; // the offset of the string to draw
    size_t span = 0; // the end point offset of each draw iteration
    size_t line = 0; // vertical line count
    size_t segment_pixel_length = 0;
    size_t char_vertical_max = 0;

    /* Example:
        "T\nEST\nU+1F642" using font::default_font. the unicode at the end is an emoji, pixel dimensions are 10x10 with spacing of 2x2
        len = 11 (6 char + 4 byte unicode + 1 nullterm)
        first iteration: chars_used = 1, codepoint = 'T', track = 0, copy "T" into vbuff + 0, span = 1, segment_pixel_length = 6, char_vertical_max = 9
        second iteration: chars_used = 1, codepoint = '\n', track = 1, copy "\n" into vbuff + 1, span = 2, segment_pixel_length = 6 - 1 = 5, char_vertical_max = 9, vbuff[2 - 1] = nullterm, draw "T"
        third iteration: chars_used = 1, codepoint = 'E', track = 2, copy "E" info vbuff + 2, span = 3, segment_pixel_length = 6, char_vertical_max = 9
        fourth iteration: chars_used = 1, codepoint = 'S', track = 3, copy "S" info vbuff + 3, span = 4, segment_pixel_length = 12, char_vertical_max = 9
        fifth iteration: chars_used = 1, codepoint = 'T', track = 4, copy "T" info vbuff + 4, span = 5, segment_pixel_length = 18, char_vertical_max = 9
        sixth iteration: chars_used = 1, codepoint = '\n', track = 5, copy "\n" into vbuff + 5, span = 6, segment_pixel_length = 18 - 1 = 17, char_vertical_max = 9, vbuff[6 - 1] = nullterm, draw "EST"
        seventh iteration: chars_used = 4, codepoint = 'U+1F642' = 0xF09F9982, track = 6, copy this into vbuff + 6, span = 10, segment_pixel_length = 12, char_vertical_max = 12
        eighth iteration: chars_used = 1, codepoint = '\0', track = 10, copy '\0' into vbuff + 10, span = 11, segment_pixel_length = 12 - 2 = 10, char_vertical_max = 12, vbuff[11 - 1] = nullterm, draw emoji
    */
    /* Example 2:
        "Hi" using font::default_font
        len = 3 (2 char + 1 nullterm)
        first iteration: chars_used = 1, codepoint = 'H', track = 0, copy "H" into vbuff + 0, span = 1, segment_pixel_length = 6, char_vertical_max = 9
        second iteration: chars_used = 1, codepoint = 'i', track = 1, copy 'i' info vbuff + 1, span = 2, segment_pixel_length = 12, char_vertical_max = 9
        third iteration: chars_used = 1, codepoint = '\0', track = 2, copy '\0' into vbuff + 2, span = 3, segment_pixel_length = 12 - 1 = 11, char_vertical_max = 9, vbuff[3 - 1] = nullterm, draw "Hi"
    */
    /* Example 3:
        "السلام عليكم" using external arabic font
        len = 25 (1 char + 12 * 2 byte unicode + 1 nullterm)
        each iteration (1st-12th): chars_used = 2, track = 0, += 2, span = 2, += 2
        last iteration (13th iteration): chars_used = 1, track = 24, span = 25, vbuff[25 - 1] = nullterm, draw the entire string

    */
    while (track < len)
    {
        size_t chars_used;
        char32_t codepoint = utf8_decode(str_iter, &chars_used);

        memcpy(vbuff + span, &codepoint, chars_used);

        span += chars_used;

        if (codepoint != '\n' && (*str_iter) != '\0') // if not new line and not last character of string
        {
            for (const Font& f : *font_group)
            {
                if (codepoint >= f.character_begin && codepoint <= f.character_end)
                {
                    selected_font = &f;
                    //printf("font data: %ix%i, %c<x<%c\n", f.char_width, f.char_height, f.character_begin, f.character_end);
                    segment_pixel_length += f.char_width + f.char_spacing_x;
    
                    if (char_vertical_max < f.char_height + f.char_spacing_y)
                        char_vertical_max = f.char_height + f.char_spacing_y;

                    break;
                }
            }
        }
        else // if new line or last character of string
        {
            vbuff[span - 1] = '\0'; // prepare for DrawText to draw from vbuff + track to vbuff + span
            segment_pixel_length -= selected_font->char_spacing_x; // remove extra spacing
            
            Vec2i32 text_offset = text_base_offset;
            text_offset.y += static_cast<int32_t>((line++) * char_vertical_max);

            switch (text_horizontal_alignment)
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

            segment_pixel_length = 0;
            char_vertical_max = 0;

            //printf("line str: %s\ttrack: %i\tspan: %i\n", vbuff + track, track, span);
            display->DrawText(screen->ToScreenCoords(origin_position) + draw_dimensions.min + text_offset, vbuff + track, *font_group, font_scale, color);
            track = span;
        }

        str_iter += chars_used;
    }

/* old algorithm using one font */
    // size_t len = strlen(text) + 1;
    // char vbuff[len];
    // size_t track = 0;
    // size_t line = 0;

    // while (track < len)
    // {
    //     /**
    //      * Example:
    //      * "Hello\n.\nWorld"
    //      * len = 14 (13 char + 1)
    //      * first iteration: segment_len = 5, copy "Hello" info vbuff, span = 5, vbuff[5] = nullterm, draw "Hello", track goes from 0 to 6
    //      * second iteration: segment_len = 1, copy "." info vbuff + 6, span = 7, vbuff[7] = nullterm, draw ".", track goes from 6 to 8
    //      * third iteration: segment_len = 5, copy "World" info vbuff + 8, span = 13, vbuff[13] = nullterm, draw "World", track goes from 8 to 13
    //      * 
    //      * Example 2:
    //      * "test\n\ntext"
    //      * len = 11
    //      * first iteration: segment_length = 4, copy "test" info vbuff, span = 4, vbuff[4] = nullterm, draw "test", track goes from 0 to 5
    //      * second iteration: segment_length = 0, copy "" info vbuff, span = 5, vbuff[5] = nullterm, draw "", track goes from 5 to 6
    //      * third iteration: segment_length = 4, copy "text" info vbuff, span = 10, vbuff[10] = nullterm, draw "text", track goes from 6 to 11
    //      */
    //     size_t segment_len = strcspn(text + track, "\n");
    //     size_t segment_pixel_length = segment_len * font->char_width + (segment_len - 1) * font->char_spacing;
    //     strncpy(vbuff + track, text + track, segment_len);

    //     size_t span = segment_len + track;
    //     vbuff[span] = '\0';

    //     Vec2i32 text_offset = text_base_offset;
    //     text_offset.y += static_cast<int32_t>((line++) * (font->char_spacing + font->char_height));
    //     switch (text_horizontal_alignment)
    //     {
    //         case AlignmentHorizontal::LEFT: break;
    //         case AlignmentHorizontal::CENTER: {
    //             text_offset.x += message_pixel_dimensions.x / 2 - segment_pixel_length / 2;
    //             break;
    //         }
    //         case AlignmentHorizontal::RIGHT: {
    //             text_offset.x += message_pixel_dimensions.x - segment_pixel_length;
    //             break;
    //         }
    //     }

    //     display->DrawText(screen->ToScreenCoords(origin_position) + draw_dimensions.min + text_offset, vbuff + track, *font_group, font_scale, color);
    //     track = span + 1;
    // }
}

void TextComponent::Align()
{
    UpdateTextDimensions();
    draw_dimensions.min = {0, 0};
    draw_dimensions.max = message_pixel_dimensions;
    Component::Align();
}

void TextBoxComponent::UpdateTextDimensions()
{
    TextComponent::UpdateTextDimensions();
    if (dynamic_box)
    {
        draw_dimensions.min = Vec2i32{0, 0};
        draw_dimensions.max = message_pixel_dimensions + padding * 2;
    }
    switch (text_vertical_alignment)
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
    switch (text_horizontal_alignment)
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

TextBoxComponent::TextBoxComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& box_dimensions, const char* text, const FontGroup* font_group, int32_t z_layer, Screen* initial_screen)
    : TextComponent(manager, origin, text, font_group, z_layer, initial_screen)
{
    draw_dimensions.max = box_dimensions;
    UpdateTextDimensions();
}
TextBoxComponent::TextBoxComponent(ScreenManager* manager, const Vec2f& screen_percentage, const Vec2i32& box_dimensions, const char* text, const FontGroup* font_group, int32_t z_layer, Screen* initial_screen)
    : TextComponent(manager, screen_percentage, text, font_group, z_layer, initial_screen)
{
    draw_dimensions.max = box_dimensions;
    UpdateTextDimensions();
}

void TextBoxComponent::Draw(const Screen* screen)
{
    Vec2i32 ss_pos = screen->ToScreenCoords(origin_position);

    if (clear_bg)
        display->FillRectangle(ss_pos + draw_dimensions.min, draw_dimensions.Size(), colors::CLEAR);

    display->DrawRectangle(ss_pos + draw_dimensions.min, draw_dimensions.Size(), color); // box
    TextComponent::Draw(screen);
}

void TextBoxComponent::Align()
{
    UpdateTextDimensions();
    Component::Align();
}

void TextBoxComponent::SetPadding(const Vec2i32& padding)
{
    this->padding = padding;
    Align();
}

void TextBoxComponent::EnableDynamicBoxDimensions(bool enable)
{
    dynamic_box = enable;
    Align();
}