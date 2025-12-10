#include <interactive-ui/SelectableComponent.h>

#include <cstring>

ComponentSelectEvent::ComponentSelectEvent(const EventSource* source, ControlAction control)
    : Event(source), control(control)
{
}

SelectableComponent::SelectableComponent(const Screen* screen, const Vec2u32& position, int32_t z_layer)
    : Component(screen, position, z_layer, true), is_hovered(false)
{
    memset(neighboring_components, 0, sizeof(neighboring_components));   
}

SelectableComponent::SelectableComponent(const Screen* screen, float x_percentage, float y_percentage, int32_t z_layer)
    : Component(screen, x_percentage, y_percentage, z_layer, true), is_hovered(false)
{
    memset(neighboring_components, 0, sizeof(neighboring_components));
}

void SelectableComponent::Update(float dt)
{
    Component::Update(dt);
    if (is_hovered)
        DrawHover();
}

void SelectableComponent::DrawHover()
{
    Vec2u32 unit{1, 1};
    data.display->DrawSquare(origin_position - unit, draw_dimensions + unit, color, true);
}

void SelectableComponent::Select0()
{
    Event* ev = new ComponentSelectEvent(this, SELECT0);
    queue_try_add(&Event::event_queue, &ev);
}

void SelectableComponent::Select1()
{
    Event* ev = new ComponentSelectEvent(this, SELECT1);
    queue_try_add(&Event::event_queue, &ev);
}