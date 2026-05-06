#pragma once

#include <vector>

#include <math/AABB.h>
#include <math/Easing.h>
#include "ControlAction.h"

#include "ScreenManager.h"
#include "iface/HoverMixin.h"

class Component;
class SelectableComponent;

enum CoordinateSpaceHorizontal
{
    LEFT_TO_RIGHT,
    RIGHT_TO_LEFT
};
enum CoordinateSpaceVertcial
{
    TOP_TO_BOTTOM,
    BOTTOM_TO_TOP
};

class Screen
{
public:
    static bool _ComponentCompare(const Component* a, const Component* b);

private:
    std::unordered_set<Component*> component_set; // stores the same values the vector does, but intended for searching
    int component_moving_reference_count = 0;

protected:
    std::vector<Component*> components;
    std::unique_ptr<HoverMixin> hover_design;

    SelectableComponent* hovered_component;
    DisplayInterface* display;
    ScreenManager* manager;

public:
    const AABBi32 dimensions;
    Vec2i32 origin_point{0, 0};
    CoordinateSpaceHorizontal cspace_h = LEFT_TO_RIGHT;
    CoordinateSpaceVertcial cspace_v = BOTTOM_TO_TOP;
    float animation_hover_duration = 0.2f;
    const FunctionLUT<float>* easing_func = &easing::lut_quad_out;

public:
    Screen(ScreenManager* manager, uint32_t width, uint32_t height);
    Screen(ScreenManager* manager, const Vec2u32& dimensions);
    Screen(const Screen& to_copy);
    virtual ~Screen() = default;

    // Should be called every time inside draw function calls.
    Vec2i32 ToScreenCoords(const Vec2i32& input_pos) const;

    inline Vec2i32 GetDimensions() const
    {
        return dimensions.max;
    }

    inline SelectableComponent* GetHoveredComponent() const
    {
        return hovered_component;
    }

    inline const std::unique_ptr<HoverMixin>& GetHoverDesign() const
    {
        return hover_design;
    }

    inline void SetHoverDesign(std::unique_ptr<HoverMixin> design)
    {
        hover_design = std::move(design);
    }
    template<class HoverImplementation> requires std::is_base_of_v<HoverMixin, HoverImplementation>
    inline void SetHoverDesign(HoverImplementation* design)
    {
        hover_design = std::make_unique<HoverImplementation>(design);
    }

    void AddComponent(Component* component);
    void RemoveComponent(Component* component);

    void HoverChange(bool instant = false) const;
    void HoverComponent(SelectableComponent* comp, bool instant = false);
    void UnhoverComponent();
    void HoverDefaultComponent();
    bool IsComponentHoverable(const SelectableComponent* comp) const;

    // Sorts components by Z-Layer. Should be called after adding components.
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