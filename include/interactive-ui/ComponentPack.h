#pragma once

#include "Component.h"

class ComponentPack : public Component
{
protected:
    std::vector<Component*> subcomponents;

public:
    ComponentPack(ScreenManager* manager, const Vec2i32& master_position, int32_t z_layer, Screen* initial_screen = nullptr);

    void Draw() override;

    void ForceVisibility(bool visibility) override;
    void SetPersonalVisibility(bool visibility) override;

    inline void AddSubcomponent(Component* component)
    {
        subcomponents.push_back(component);
    }

    void SortSubcomponents();

};