#include <interactive-ui/Component.h>

Component::Component(const ScreenManager* manager, const Vec2u32& position, int32_t z_layer, const Screen* initial_screen, bool selectable)
    : manager(manager), display(manager->GetDisplay()), z_layer(z_layer), color(0xFFFFFFFF), selectable(selectable), forced_visibility(false), personal_visibility(true)
{
    if (initial_screen)
    {
        Vec2u32 screen_dim = initial_screen->GetDimensions();
        origin_position.x = position.x > screen_dim.x ? screen_dim.x : position.x;   
        origin_position.y = position.y > screen_dim.y ? screen_dim.y : position.y;
    }
    else
        origin_position = position;
}

Component::Component(const ScreenManager* manager, float x_percentage, float y_percentage, int32_t z_layer, const Screen* initial_screen, bool selectable)
    : Component(manager, {
        static_cast<uint32_t>(initial_screen->GetDimensions().x * x_percentage),
        static_cast<uint32_t>(initial_screen->GetDimensions().y * y_percentage)
    }, z_layer, initial_screen, selectable)
{
}

void Component::Update(float dt)
{
    if (forced_visibility && personal_visibility)
        Draw();
}