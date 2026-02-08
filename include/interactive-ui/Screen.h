#pragma once

#include <vector>

#include <math/AABB.h>
#include "ControlAction.h"

#include "ScreenManager.h"

class Component;
class PaddingComponent;
class SelectableComponent;

class Screen
{
public:
    static bool _ComponentCompare(const Component* a, const Component* b);

private:
    std::unordered_set<Component*> component_set; // stores the same values the vector does, but intended for searching
    bool allow_hover_draw = false;

    int component_moving_reference_count = 0;

protected:
    const AABBi32 dimensions;

    std::vector<Component*> components;

    PaddingComponent* hover_design;
    SelectableComponent* hovered_component;
    
    DisplayInterface* display;
    ScreenManager* manager;

    float animation_hover_duration = 0.25f;

    void HoverChange(bool instant = false);

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

    void HoverComponent(SelectableComponent* comp, bool instant = false);
    void UnhoverComponent();
    void HoverDefaultComponent();
    bool IsComponentHoverable(const SelectableComponent* comp) const;
    inline void SetHoverAnimationDuration(float duration_s)
    {
        animation_hover_duration = duration_s;

    }

    void SortComponents();
    void SetComponentZLayer(Component* comp, int32_t z_layer);

    virtual void OnControl(uint32_t control_mask);
    virtual void OnScreenSelect();
    virtual void OnScreenDeselect();
    
    virtual void Update(float dt);

    bool NavigateToComponent(uint32_t control_mask);
    void ActOnComponent(uint32_t control_mask);

    inline bool HasMovingComponent() const
    {
        return component_moving_reference_count > 0;
    }

    friend ScreenManager;
    friend Component;
};