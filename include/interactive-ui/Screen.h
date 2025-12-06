#pragma once

#include <vector>

#include <pico/util/queue.h>

#include <math/Vec2.h>
#include "ControlAction.h"

#include "ScreenData.h"

class Component;
class SelectableComponent;

class Screen
{
public:
    static bool _ComponentCompare(const Component* a, const Component* b);

protected:
    const Vec2u32 dimensions;

    std::vector<Component*> components;

    SelectableComponent* hovered_component;
    
    ScreenData data;

public:
    Screen(ScreenManager* manager, uint32_t width, uint32_t height);
    Screen(ScreenManager* manager, const Vec2u32& dimensions);

    inline Vec2u32 GetDimensions() const
    {
        return dimensions;
    }

    inline SelectableComponent* GetHoveredComponent() const
    {
        return hovered_component;
    }

    void AddComponent(Component* component);

    void SortComponents();

    virtual void OnControl(uint32_t control_mask);
    virtual void OnScreenSelect();
    virtual void OnScreenDeselect();
    
    virtual void Update(float dt);

    bool NavigateToComponent(uint32_t control_mask);

    friend ScreenManager;
    friend Component;
};