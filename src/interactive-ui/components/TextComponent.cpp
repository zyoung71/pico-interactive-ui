#include <interactive-ui/components/TextComponent.h>

#include <cstring>

TextComponent::TextComponent(ScreenManager* manager, const Vec2i32& origin, const char* text, const Font* font, int32_t z_layer, const Screen* initial_screen)
    : SelectableComponent(manager, origin, z_layer, initial_screen), text(text), font(font)
{
    const size_t msg_len = strlen(text);
    lines = 1;

    size_t segment_max = 0;
    size_t iter = 0;

    for (size_t c = 0; c < msg_len; c++) //  <---- DID YOU SEE THAT???
    {
        size_t len = strcspn(text + iter, "\n");
        if (len > segment_max)
            segment_max = len;

        iter += len;

        if (text[c] == '\n')
        {
            iter++;
            lines++;
        }
    }

    size_t message_pixel_length = segment_max * font->char_width + (segment_max - 1) * font->char_spacing;
    draw_dimensions = {(int32_t)message_pixel_length, static_cast<int32_t>(font->char_height * lines + font->char_spacing * (lines - 1))};
}
TextComponent::TextComponent(ScreenManager* manager, float x_percentage, float y_percentage, const char* text, const Font* font, int32_t z_layer, const Screen* initial_screen)
    : SelectableComponent(manager, x_percentage, y_percentage, z_layer, initial_screen), text(text), font(font)
{
    const size_t msg_len = strlen(text);
    lines = 1;

    size_t segment_max = 0;
    size_t iter = 0;

    for (size_t c = 0; c < msg_len; c++) //  <---- DID YOU SEE THAT???
    {
        size_t len = strcspn(text + iter, "\n");
        if (len > segment_max)
            segment_max = len;

        iter += len;

        if (text[c] == '\n')
        {
            iter++;
            lines++;
        }
    }

    size_t message_pixel_length = segment_max * font->char_width + (segment_max - 1) * font->char_spacing;
    draw_dimensions = {(int32_t)message_pixel_length, static_cast<int32_t>(font->char_height * lines + font->char_spacing * (lines - 1))};
}

void TextComponent::Draw()
{
    display->DrawText(origin_position, text, font, color);
}

TextBoxComponent::TextBoxComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& dimensions, const Vec2i32& padding, const char* text, const Font* font, int32_t z_layer, const Screen* initial_screen)
    : TextComponent(manager, origin, text, font, z_layer, initial_screen), padding(padding)
{
    draw_dimensions = dimensions;
}
TextBoxComponent::TextBoxComponent(ScreenManager* manager, float x_percentage, float y_percentage, const Vec2i32& dimensions, const Vec2i32& padding, const char* text, const Font* font, int32_t z_layer, const Screen* initial_screen)
    : TextComponent(manager, x_percentage, y_percentage, text, font, z_layer, initial_screen), padding(padding)
{
    draw_dimensions = dimensions;
}
TextBoxComponent::TextBoxComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& dimensions, float x_pad_percentage, float y_pad_percentage, const char* text, const Font* font, int32_t z_layer, const Screen* initial_screen)
    : TextComponent(manager, origin, text, font, z_layer, initial_screen), padding({static_cast<int32_t>(dimensions.x * x_pad_percentage), static_cast<int32_t>(dimensions.y * y_pad_percentage)})
{
    draw_dimensions = dimensions;
}
TextBoxComponent::TextBoxComponent(ScreenManager* manager, float x_percentage, float y_percentage, const Vec2i32& dimensions, float x_pad_percentage, float y_pad_percentage, const char* text, const Font* font, int32_t z_layer, const Screen* initial_screen)
    : TextComponent(manager, x_percentage, y_percentage, text, font, z_layer, initial_screen), padding({static_cast<int32_t>(dimensions.x * x_pad_percentage), static_cast<int32_t>(dimensions.y * y_pad_percentage)})
{
    draw_dimensions = dimensions;
}

void TextBoxComponent::Draw()
{
    display->DrawSquare(origin_position, draw_dimensions, color, true); // box
    display->DrawText(origin_position + padding, text, font, color); // text
}