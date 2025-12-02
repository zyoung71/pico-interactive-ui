#include <interactive-ui/SelectableComponent.h>

SelectableComponent::SelectableComponent(Screen* screen, const Vec2u32& position)
    : Component(screen, position)
{
}

SelectableComponent::SelectableComponent(Screen* screen, float x_percentage, float y_percentage)
    : Component(screen, x_percentage, y_percentage)
{
}