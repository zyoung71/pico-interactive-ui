#include <interactive-ui/ComponentPack.h>

#include <algorithm>

ComponentPack::ComponentPack(ScreenManager* manager, const Vec2i32& master_position, int32_t z_layer, Screen* initial_screen)
    : Component(manager, master_position, z_layer, initial_screen)
{
}

void ComponentPack::Update(float dt, const Screen* screen)
{
    Component::Update(dt, screen);
    for (auto c : subcomponents)
        c->Update(dt, screen);
}

void ComponentPack::Draw()
{
    for (auto c : subcomponents)
        c->Draw();
}

void ComponentPack::Align()
{
    for (auto c : subcomponents)
        c->Align();
    Component::Align();
}

void ComponentPack::Scale(const Vec2f& scale_vec)
{
    for (auto c : subcomponents)
        c->Scale(scale_vec);
    Component::Scale(scale_vec);
}

void ComponentPack::ForceVisibility(bool visibility)
{
    for (auto c : subcomponents)
        c->ForceVisibility(visibility);
    Component::ForceVisibility(visibility);
}

void ComponentPack::SetPersonalVisibility(bool visibility)
{
    for (auto c : subcomponents)
        c->SetPersonalVisibility(visibility);
    Component::SetPersonalVisibility(visibility);
}

void ComponentPack::AddSubcomponent(Component* comp)
{
    subcomponents.push_back(comp);
}

void ComponentPack::SortSubcomponents()
{
    std::sort(subcomponents.begin(), subcomponents.end(), Screen::_ComponentCompare);
    Vec2i32 low, high;
    for (auto c : subcomponents)
    {
        Vec2i32 origin = c->GetOriginPosition();
        if (origin.x < low.x)
            low.x = origin.x;
        if (origin.y < low.y)
            low.y = origin.y;
        if (origin.x > high.x)
            high.x = origin.x;
        if (origin.y > high.y)
            high.y = origin.y;
    }
    draw_dimensions.min = low;
    draw_dimensions.max = high;
}