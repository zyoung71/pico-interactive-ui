#include <interactive-ui/SelectableComponent.h>

SelectableComponent::SelectableComponent(Screen* screen, const Vec2u32& position, int32_t z_layer)
    : Component(screen, position, z_layer), is_selected(false)
{
}

SelectableComponent::SelectableComponent(Screen* screen, float x_percentage, float y_percentage, int32_t z_layer)
    : Component(screen, x_percentage, y_percentage, z_layer), is_selected(false)
{
}

void SelectableComponent::Update(float dt)
{
    Component::Update(dt);
    if (is_selected)
        DrawSelection();
}

void SelectableComponent::DrawSelection()
{
    Vec2u32 unit{1, 1};
    data.display->DrawSquare(origin_position - unit, draw_dimensions + unit, color, true);
}