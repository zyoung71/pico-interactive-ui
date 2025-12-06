#include <interactive-ui/Component.h>

Component::Component(Screen* screen, const Vec2u32& position, int32_t z_layer, bool selectable)
    : data(screen->data), z_layer(z_layer), color(0xFFFFFFFF), selectable(selectable), forced_visibility(false), personal_visibility(true)
{
    Vec2u32 screen_dim = screen->GetDimensions();
    origin_position.x = position.x > screen_dim.x ? screen_dim.x : position.x;
    origin_position.y = position.y > screen_dim.y ? screen_dim.y : position.y;

}

Component::Component(Screen* screen, float x_percentage, float y_percentage, int32_t z_layer, bool selectable)
    : Component(screen, {
        static_cast<uint32_t>(screen->GetDimensions().x * x_percentage),
        static_cast<uint32_t>(screen->GetDimensions().y * y_percentage)
    }, z_layer, selectable)
{
}

void Component::Update(float dt)
{
    if (forced_visibility && personal_visibility)
        Draw();
}