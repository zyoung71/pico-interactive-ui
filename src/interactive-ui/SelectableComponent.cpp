#include <interactive-ui/SelectableComponent.h>

#include <cstring>

ComponentSelectEvent::ComponentSelectEvent(EventSource* source, ControlAction control)
    : Event(source), control(control)
{
}

SelectableComponent::SelectableComponent(ScreenManager* manager, const Vec2i32& position, int32_t z_layer, Screen* initial_screen)
    : Component(manager, position, z_layer, initial_screen, true), allow_hover_draw(true), cancel_master_back_action(false)
{
    if (initial_screen)
        component_lut[initial_screen] = SelectionTable{nullptr, nullptr, nullptr, nullptr};
}

SelectableComponent::SelectableComponent(ScreenManager* manager, float x_percentage, float y_percentage, int32_t z_layer, Screen* initial_screen)
    : Component(manager, x_percentage, y_percentage, z_layer, initial_screen, true), allow_hover_draw(true), cancel_master_back_action(false)
{
    if (initial_screen)
        component_lut[initial_screen] = SelectionTable{nullptr, nullptr, nullptr, nullptr};
}

void SelectableComponent::AddComponentTable(const Screen* screen, SelectableComponent* up, SelectableComponent* down, SelectableComponent* left, SelectableComponent* right)
{
    if (screen)
        component_lut[screen] = SelectionTable{up, down, left, right};
}

void SelectableComponent::AddComponentTable(const Screen* screen, SelectableComponent** neighbors)
{
    if (screen)
        component_lut[screen] = SelectionTable{neighbors[0], neighbors[1], neighbors[2], neighbors[3]};
}

void SelectableComponent::DrawHover()
{
    // draw_dimensions is inclusive of the origin position pixel, so the bounds must extend 2 pixels on the ending side
    display->DrawSquare(origin_position + draw_dimensions.min - Vec2i32{1, 1}, draw_dimensions.max - draw_dimensions.min + Vec2i32{2, 2}, color, true);
}

void SelectableComponent::Control(ControlAction action)
{
    Event* ev = new ComponentSelectEvent(this, action);
    queue_try_add(&Event::event_queue, &ev);
}