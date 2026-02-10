#pragma once

#include "Component.h"

class ComponentPack : public Component
{
protected:
    std::vector<Component*> subcomponents;

public:
    ComponentPack(ScreenManager* manager, const Vec2i32& master_position, int32_t z_layer, Screen* initial_screen = nullptr);
    virtual ~ComponentPack() = default;

    virtual void Update(float dt, const Screen* screen) override;
    virtual void Draw() override;
    virtual void Align() override;
    virtual void Scale() override;

    virtual void ForceVisibility(bool visibility) override;
    virtual void SetPersonalVisibility(bool visibility) override;

    virtual void AddSubcomponent(Component* component);
    void SortSubcomponents();

};