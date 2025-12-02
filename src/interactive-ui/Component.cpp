#include <interactive-ui/Component.h>

Component::Component(Screen* screen, const Vec2u32& position)
    : data(screen->data)
{
    Vec2u32 screen_dim = screen->GetDimensions();
    origin_position.x = position.x > screen_dim.x ? screen_dim.x : position.x;
    origin_position.y = position.y > screen_dim.y ? screen_dim.y : position.y;

}

Component::Component(Screen* screen, float x_percentage, float y_percentage)
    : Component(screen, {
        static_cast<uint32_t>(screen->GetDimensions().x * x_percentage),
        static_cast<uint32_t>(screen->GetDimensions().y * y_percentage)
    })
{
}

Component::~Component()
{
    data.screen->RemoveComponent(this);
}

void Component::Update(float dt)
{
    if (is_visible)
        Draw();
}