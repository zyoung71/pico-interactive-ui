#pragma once

#include "Component.h"

#include <event/Event.h>

class ComponentSelectEvent : public Event
{
protected:
    ControlAction control;

public:
    ComponentSelectEvent(const EventSource* source, ControlAction control);
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
    // Contains a 4-directional table for other components for each screen. Some components may be used across screens, so a map is necessary.
    std::unordered_map<const Screen*, SelectionTable> component_lut;

public:
    SelectableComponent(const Screen* screen, const Vec2u32& position, int32_t z_layer);
    SelectableComponent(const Screen* screen, float x_percentage, float y_percentage, int32_t z_layer);
    virtual ~SelectableComponent() = default;

    void AddComponentTable(const Screen* screen, SelectableComponent* up = nullptr, SelectableComponent* down = nullptr, SelectableComponent* left = nullptr, SelectableComponent* right = nullptr);
    void AddComponentTable(const Screen* screen, SelectableComponent** neighbors); // Must be of size 4.

    virtual void DrawHover(); // Draw the extra elements when the component is hovered. Default is an outlining square.
    virtual void OnComponentHovered() {};
    virtual void OnComponentUnhovered() {};

    // What happens when the hovered component is actually selected.
    // Think of it as a left-click.
    virtual void Select0();

    // Second action when the hovered component is actually selected.
    // Think of it as a right-click.
    virtual void Select1();
};