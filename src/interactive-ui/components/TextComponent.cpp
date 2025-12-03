#include <interactive-ui/components/TextComponent.h>

#include <cstring>

TextComponent::TextComponent(Screen* screen, const Vec2u32& origin, const TextProperties& props, int32_t z_layer)
    : SelectableComponent(screen, origin, z_layer), text_properties(props)
{
    size_t msg_len = strlen(props.text);
    text_properties.lines = 1;
    for (size_t c = 0; c < msg_len; c++) //  <---- DID YOU SEE THAT???
    {
        if (props.text[c] == '\n')
            text_properties.lines++;
    }

    size_t message_pixel_length = (msg_len * props.width + (msg_len - 1) * props.spacing) / props.lines;
    draw_dimensions = {message_pixel_length, props.height * props.lines + props.spacing * (props.lines - 1)};
}
TextComponent::TextComponent(Screen* screen, float x_percentage, float y_percentage, const TextProperties& props, int32_t z_layer)
    : SelectableComponent(screen, x_percentage, y_percentage, z_layer), text_properties(props)
{
    size_t msg_len = strlen(props.text);
    text_properties.lines = 1;
    for (size_t c = 0; c < msg_len; c++)
    {
        if (props.text[c] == '\n')
            text_properties.lines++;
    }
    
    size_t message_pixel_length = (msg_len * props.width + (msg_len - 1) * props.spacing) / props.lines;
    draw_dimensions = {message_pixel_length, props.height * props.lines + props.spacing * (props.lines - 1)};
}

void TextComponent::Draw()
{
    data.display->DrawText(origin_position, text_properties, color);
}

void TextComponent::DrawSelection()
{
    // Draw a square around the text.
    data.display->DrawSquare(origin_position - Vec2u32{1, 1}, draw_dimensions + Vec2u32{1, 1}, color, true);
}

TextBoxComponent::TextBoxComponent(Screen* screen, const Vec2u32& origin, const Vec2u32& dimensions, const Vec2u32& padding, const TextProperties& props, int32_t z_layer)
    : TextComponent(screen, origin, props, z_layer), padding(padding)
{
    draw_dimensions = dimensions;
}
TextBoxComponent::TextBoxComponent(Screen* screen, float x_percentage, float y_percentage, const Vec2u32& dimensions, const Vec2u32& padding, const TextProperties& props, int32_t z_layer)
    : TextComponent(screen, x_percentage, y_percentage, props, z_layer), padding(padding)
{
    draw_dimensions = dimensions;
}
TextBoxComponent::TextBoxComponent(Screen* screen, const Vec2u32& origin, const Vec2u32& dimensions, float x_pad_percentage, float y_pad_percentage, const TextProperties& props, int32_t z_layer)
    : TextComponent(screen, origin, props, z_layer), padding({static_cast<uint32_t>(dimensions.x * x_pad_percentage), static_cast<uint32_t>(dimensions.y * y_pad_percentage)})
{
    draw_dimensions = dimensions;
}
TextBoxComponent::TextBoxComponent(Screen* screen, float x_percentage, float y_percentage, const Vec2u32& dimensions, float x_pad_percentage, float y_pad_percentage, const TextProperties& props, int32_t z_layer)
    : TextComponent(screen, x_percentage, y_percentage, props, z_layer), padding({static_cast<uint32_t>(dimensions.x * x_pad_percentage), static_cast<uint32_t>(dimensions.y * y_pad_percentage)})
{
    draw_dimensions = dimensions;
}

void TextBoxComponent::Draw()
{
    data.display->DrawSquare(origin_position, draw_dimensions, color, true); // box
    data.display->DrawText(origin_position + padding, text_properties, color); // text
}