#include <interactive-ui/components/TextComponent.h>

#include <cstring>

TextComponent::TextComponent(Screen* screen, const Vec2u32& origin, const TextProperties& props, int32_t z_layer)
    : SelectableComponent(screen, origin, z_layer), text_properties(props)
{
    const size_t msg_len = strlen(props.text);
    text_properties.lines = 1;

    size_t segment_max = 0;
    size_t iter = 0;

    for (size_t c = 0; c < msg_len; c++) //  <---- DID YOU SEE THAT???
    {
        size_t len = strcspn(props.text + iter, "\n");
        if (len > segment_max)
            segment_max = len;

        iter += len;

        if (props.text[c] == '\n')
        {
            iter++;
            text_properties.lines++;
        }
    }

    size_t message_pixel_length = segment_max * props.width + (segment_max - 1) * props.spacing;
    draw_dimensions = {message_pixel_length, props.height * props.lines + props.spacing * (props.lines - 1)};
}
TextComponent::TextComponent(Screen* screen, float x_percentage, float y_percentage, const TextProperties& props, int32_t z_layer)
    : SelectableComponent(screen, x_percentage, y_percentage, z_layer), text_properties(props)
{
    const size_t msg_len = strlen(props.text);
    text_properties.lines = 1;

    size_t segment_max = 0;
    size_t iter = 0;

    for (size_t c = 0; c < msg_len; c++)
    {
        size_t len = strcspn(props.text + iter, "\n");
        if (len > segment_max)
            segment_max = len;

        iter += len;

        if (props.text[c] == '\n')
        {
            iter++;
            text_properties.lines++;
        }
    }

    size_t message_pixel_length = segment_max * props.width + (segment_max - 1) * props.spacing;
    draw_dimensions = {message_pixel_length, props.height * props.lines + props.spacing * (props.lines - 1)};
}

void TextComponent::Draw()
{
    data.display->DrawText(origin_position, text_properties, color);
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