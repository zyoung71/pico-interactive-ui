#pragma once

#include "Component.h"

#include <event/Event.h>

class ComponentSelectEvent : public Event
{
protected:
    ControlAction control;

public:
    ComponentSelectEvent(EventSource* source, ControlAction control);

    // Will guarantee only ONE control. For actions that caused multiple control queues at once, it will repeatedly
    // call the callbacks that this event is found in.
    ControlAction GetControl() const
    {
        return control;
    }
};

class SelectableComponent;

struct SelectionTable
{
    union
    {
        SelectableComponent* neighboring_components[4];
        struct
        {
            SelectableComponent* up_component;
            SelectableComponent* down_component;
            SelectableComponent* left_component;
            SelectableComponent* right_component;
        };
    };
};

class SelectableComponent : public Component, public EventSource
{
public:
    typedef ComponentSelectEvent EventType;

protected:
    bool cancel_master_back_action;
    bool locked = false;

public:
    // Contains a 4-directional table for other components for each screen. Some components may be used across screens, so a map is necessary.
    std::unordered_map<const Screen*, SelectionTable> component_lut;

public:
    SelectableComponent(ScreenManager* manager, const Vec2i32& position, int32_t z_layer, Screen* initial_screen = nullptr);
    SelectableComponent(ScreenManager* manager, const Vec2f& screen_percentage, int32_t z_layer, Screen* initial_screen);
    virtual ~SelectableComponent() = default;

    void AddComponentTable(const Screen* screen, SelectableComponent* up = nullptr, SelectableComponent* down = nullptr, SelectableComponent* left = nullptr, SelectableComponent* right = nullptr);
    void AddComponentTable(const Screen* screen, SelectableComponent** neighbors); // Must be of size 4.
    
    // @return True if a lock or unlock was successful
    bool Lock(bool lock);
    inline bool IsLocked() const
    {
        return locked;
    }
    inline bool LockToggle()
    {
        return Lock(!locked);
    }

    virtual void OnComponentHovered() {};
    virtual void OnComponentUnhovered() {};

    virtual void Control(ControlAction action);

    inline void CancelMasterBackAction(bool cancel = true)
    {
        cancel_master_back_action = cancel;
    }

    friend Screen;
};