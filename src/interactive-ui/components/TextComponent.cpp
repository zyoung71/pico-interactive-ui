#include <interactive-ui/components/TextComponent.h>

#include <cstring>

TextComponent::TextComponent(Screen* screen, const Vec2u32& origin, const TextProperties& props, int32_t z_layer)
    : SelectableComponent(screen, origin, z_layer), text_properties(props)
{
    size_t msg_len = strlen(props.text);
    message_pixel_length = (msg_len * props.width + (msg_len - 1) * props.spacing) / props.lines;
    draw_dimensions = {message_pixel_length / props.lines, props.height * props.lines};
}
TextComponent::TextComponent(Screen* screen, float x_percentage, float y_percentage, const TextProperties& props, int32_t z_layer)
    : SelectableComponent(screen, x_percentage, y_percentage, z_layer), text_properties(props)
{
    size_t msg_len = strlen(props.text);
    message_pixel_length = (msg_len * props.width + (msg_len - 1) * props.spacing) / props.lines;
    draw_dimensions = {message_pixel_length / props.lines, props.height * props.lines + props.spacing * (props.lines - 1)};
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
    Vec2u32 range_local = draw_dimensions - padding * 2; // maximum pixel length/height for text
    text_properties.lines = message_pixel_length / range_local.x;

    if (message_pixel_length % range_local.x > 0) // If doesn't fit perfectly
        text_properties.lines++; // add a line

    size_t height_per_line = text_properties.height + text_properties.spacing;
    if (height_per_line > range_local.y / text_properties.lines) // clamp if height conflicts
    {
        text_properties.lines = range_local.y / height_per_line;
    }
}
TextBoxComponent::TextBoxComponent(Screen* screen, float x_percentage, float y_percentage, const Vec2u32& dimensions, const Vec2u32& padding, const TextProperties& props, int32_t z_layer)
    : TextComponent(screen, x_percentage, y_percentage, props, z_layer), padding(padding)
{
    draw_dimensions = dimensions;
    Vec2u32 range_local = draw_dimensions - padding * 2; // maximum pixel length/height for text
    text_properties.lines = message_pixel_length / range_local.x;

    if (message_pixel_length % range_local.x > 0) // If doesn't fit perfectly
        text_properties.lines++; // add a line

    size_t height_per_line = text_properties.height + text_properties.spacing;
    if (height_per_line > range_local.y / text_properties.lines) // clamp if height conflicts
    {
        text_properties.lines = range_local.y / height_per_line;
    }
}
TextBoxComponent::TextBoxComponent(Screen* screen, const Vec2u32& origin, const Vec2u32& dimensions, float x_pad_percentage, float y_pad_percentage, const TextProperties& props, int32_t z_layer)
    : TextComponent(screen, origin, props, z_layer), padding({static_cast<uint32_t>(dimensions.x * x_pad_percentage), static_cast<uint32_t>(dimensions.y * y_pad_percentage)})
{
    draw_dimensions = dimensions;
    Vec2u32 range_local = draw_dimensions - padding * 2; // maximum pixel length/height for text
    text_properties.lines = message_pixel_length / range_local.x;

    if (message_pixel_length % range_local.x > 0) // If doesn't fit perfectly
        text_properties.lines++; // add a line

    size_t height_per_line = text_properties.height + text_properties.spacing;
    if (height_per_line > range_local.y / text_properties.lines) // clamp if height conflicts
    {
        text_properties.lines = range_local.y / height_per_line;
    }
}
TextBoxComponent::TextBoxComponent(Screen* screen, float x_percentage, float y_percentage, const Vec2u32& dimensions, float x_pad_percentage, float y_pad_percentage, const TextProperties& props, int32_t z_layer)
    : TextComponent(screen, x_percentage, y_percentage, props, z_layer), padding({static_cast<uint32_t>(dimensions.x * x_pad_percentage), static_cast<uint32_t>(dimensions.y * y_pad_percentage)})
{
    draw_dimensions = dimensions;
    Vec2u32 range_local = draw_dimensions - padding * 2; // maximum pixel length/height for text
    text_properties.lines = message_pixel_length / range_local.x;

    if (message_pixel_length % range_local.x > 0) // If doesn't fit perfectly
        text_properties.lines++; // add a line

    size_t height_per_line = text_properties.height + text_properties.spacing;
    if (height_per_line > range_local.y / text_properties.lines) // clamp if height conflicts
    {
        text_properties.lines = range_local.y / height_per_line;
    }
}

void TextBoxComponent::Draw()
{
    data.display->DrawSquare(origin_position, draw_dimensions, color, true); // box
    data.display->DrawText(origin_position + padding, text_properties, color); // text
}