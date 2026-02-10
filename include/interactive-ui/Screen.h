#pragma once

#include <vector>

#include <math/AABB.h>
#include <math/Graphics.h>
#include "ControlAction.h"

#include "ScreenManager.h"

class Component;
class SelectableComponent;
class PaddingComponent;

class Screen
{
public:
    static bool _ComponentCompare(const Component* a, const Component* b);
    static constexpr int32_t hover_z_layer = 5000;

private:
    std::unordered_set<Component*> component_set; // stores the same values the vector does, but intended for searching
    int component_moving_reference_count = 0;

protected:
    std::vector<Component*> components;
    PaddingComponent* hover_design;
    SelectableComponent* hovered_component;
    DisplayInterface* display;
    ScreenManager* manager;

public:
    const AABBi32 dimensions;
    float animation_hover_duration = 0.2f;
    const graphics::easing::EasingFunctionLUT* easing_func = &graphics::easing::lut_quad_out;

public:
    Screen(ScreenManager* manager, uint32_t width, uint32_t height);
    Screen(ScreenManager* manager, const Vec2u32& dimensions);
    Screen(const Screen& to_copy);
    virtual ~Screen();

    inline Vec2i32 GetDimensions() const
    {
        return dimensions.max;
    }

    inline SelectableComponent* GetHoveredComponent() const
    {
        return hovered_component;
    }

    void AddComponent(Component* component);
    void RemoveComponent(Component* component);

    void HoverChange(bool instant = false) const;
    void HoverComponent(SelectableComponent* comp, bool instant = false);
    void UnhoverComponent();
    void HoverDefaultComponent();
    bool IsComponentHoverable(const SelectableComponent* comp) const;

    void SortComponents();

    virtual void OnControl(uint64_t control_mask);
    virtual void OnScreenSelect();
    virtual void OnScreenDeselect();
    
    void ProcessQueuedControls();
    virtual void Update(float dt);

    bool NavigateToComponent(uint64_t control_mask);
    void ActOnComponent(uint64_t control_mask);

    inline bool HasMovingComponent() const
    {
        return component_moving_reference_count > 0;
    }

    friend ScreenManager;
    friend Component;
    friend SelectableComponent;
};