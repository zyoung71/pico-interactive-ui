#include <interactive-ui/SelectableComponent.h>
#include <interactive-ui/components/PaddingComponent.h>

#include <cstring>

ComponentSelectEvent::ComponentSelectEvent(EventSource* source, uint64_t control)
    : Event(source), control(control)
{
}

SelectableComponent::SelectableComponent(ScreenManager* manager, const Vec2i32& position, int32_t z_layer, Screen* initial_screen)
    : Component(manager, position, z_layer, initial_screen, true)
{
    if (initial_screen)
        component_lut[initial_screen] = SelectionTable{nullptr, nullptr, nullptr, nullptr};
}

SelectableComponent::SelectableComponent(ScreenManager* manager, const Vec2f& screen_percentage, int32_t z_layer, Screen* initial_screen)
    : Component(manager, screen_percentage, z_layer, initial_screen, true)
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

void SelectableComponent::Control(uint64_t action)
{
    // does not throw events onto the queue, instead immediately processes them
    ComponentSelectEvent ev = ComponentSelectEvent(this, action);
    for (auto&& act : event_actions)
    {
        act.action(&ev, act.user_data);
    }
}

bool SelectableComponent::Lock(bool lock)
{
    // we know components should be unlocked if not on screen and should not ever be locked if so
    if (screen_set.contains(manager->GetCurrentScreen())) // failsafe just in case
    {
        locked = lock;
        cancel_master_back_action = lock;
        manager->GetCurrentScreen()->hover_design->thickness = lock ? 2 : 1;
        return true;
    }
    return false;
}