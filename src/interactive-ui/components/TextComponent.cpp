#include <interactive-ui/components/TextComponent.h>

#include <cstring>

size_t TextComponent::UpdateDrawDimensions()
{
    const size_t msg_len = strlen(text);
    lines = 1;

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

    return segment_max;
}

TextComponent::TextComponent(ScreenManager* manager, const Vec2i32& origin, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen)
    : SelectableComponent(manager, origin, z_layer, initial_screen), text(text), font(font)
{
    size_t segment_max = UpdateDrawDimensions();
    size_t message_pixel_length = segment_max * font->char_width + (segment_max - 1) * font->char_spacing;
    draw_dimensions.max = {(int32_t)message_pixel_length, static_cast<int32_t>(font->char_height * lines + font->char_spacing * (lines - 1))};
}
TextComponent::TextComponent(ScreenManager* manager, float x_percentage, float y_percentage, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen)
    : SelectableComponent(manager, x_percentage, y_percentage, z_layer, initial_screen), text(text), font(font)
{
    size_t segment_max = UpdateDrawDimensions();
    size_t message_pixel_length = segment_max * font->char_width + (segment_max - 1) * font->char_spacing;
    draw_dimensions.max = {(int32_t)message_pixel_length, static_cast<int32_t>(font->char_height * lines + font->char_spacing * (lines - 1))};
}

void TextComponent::Draw()
{
    display->DrawText(origin_position, text, font, color);
}

TextBoxComponent::TextBoxComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& dimensions, const Vec2i32& padding, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen)
    : TextComponent(manager, origin, text, font, z_layer, initial_screen), padding(padding)
{
    draw_dimensions.max = dimensions;
}
TextBoxComponent::TextBoxComponent(ScreenManager* manager, float x_percentage, float y_percentage, const Vec2i32& dimensions, const Vec2i32& padding, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen)
    : TextComponent(manager, x_percentage, y_percentage, text, font, z_layer, initial_screen), padding(padding)
{
    draw_dimensions.max = dimensions;
}
TextBoxComponent::TextBoxComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& dimensions, float x_pad_percentage, float y_pad_percentage, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen)
    : TextComponent(manager, origin, text, font, z_layer, initial_screen), padding({static_cast<int32_t>(dimensions.x * x_pad_percentage), static_cast<int32_t>(dimensions.y * y_pad_percentage)})
{
    draw_dimensions.max = dimensions;
}
TextBoxComponent::TextBoxComponent(ScreenManager* manager, float x_percentage, float y_percentage, const Vec2i32& dimensions, float x_pad_percentage, float y_pad_percentage, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen)
    : TextComponent(manager, x_percentage, y_percentage, text, font, z_layer, initial_screen), padding({static_cast<int32_t>(dimensions.x * x_pad_percentage), static_cast<int32_t>(dimensions.y * y_pad_percentage)})
{
    draw_dimensions.max = dimensions;
}

void TextBoxComponent::Draw()
{
    display->DrawSquare(origin_position, draw_dimensions.max, color, true); // box
    display->DrawText(origin_position + padding, text, font, color); // text
}