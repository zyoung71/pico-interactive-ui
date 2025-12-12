#include <interactive-ui/SelectableComponent.h>

#include <cstring>

ComponentSelectEvent::ComponentSelectEvent(const EventSource* source, ControlAction control)
    : Event(source), control(control)
{
}

SelectableComponent::SelectableComponent(const ScreenManager* manager, const Vec2u32& position, int32_t z_layer, const Screen* initial_screen)
    : Component(manager, position, z_layer, initial_screen, true)
{
    if (initial_screen)
    {
        component_lut.emplace(initial_screen, SelectionTable{nullptr, nullptr, nullptr, nullptr});
    }  
}

SelectableComponent::SelectableComponent(const ScreenManager* manager, float x_percentage, float y_percentage, int32_t z_layer, const Screen* initial_screen)
    : Component(manager, x_percentage, y_percentage, z_layer, initial_screen, true)
{
    if (initial_screen)
    {
        component_lut.emplace(initial_screen, SelectionTable{nullptr, nullptr, nullptr, nullptr});
    }  
}

void SelectableComponent::AddComponentTable(const Screen* screen, SelectableComponent* up, SelectableComponent* down, SelectableComponent* left, SelectableComponent* right)
{
    if (screen)
    {
        component_lut.emplace(screen, SelectionTable{up, down, left, right});
    }
}

void SelectableComponent::AddComponentTable(const Screen* screen, SelectableComponent** neighbors)
{
    if (screen)
    {
        component_lut.emplace(screen, SelectionTable{neighbors[0], neighbors[1], neighbors[2], neighbors[3]});
    }
}

void SelectableComponent::DrawHover()
{
    Vec2u32 unit{1, 1};
    display->DrawSquare(origin_position - unit, draw_dimensions + unit, color, true);
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