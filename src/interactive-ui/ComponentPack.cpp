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

void ComponentPack::Draw(const Screen* screen)
{
    for (auto c : subcomponents)
        c->Draw(screen);
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
    AABBi32 c_dims;
    for (auto c : subcomponents)
    {
        c_dims = c->GetDrawDimensions();
        if (c_dims.xmin < draw_dimensions.xmin)
            draw_dimensions.xmin = c_dims.xmin;
        if (c_dims.ymin < draw_dimensions.ymin)
            draw_dimensions.ymin = c_dims.ymin;
        if (c_dims.xmax > draw_dimensions.xmax)
            draw_dimensions.xmax = c_dims.xmax;
        if (c_dims.ymax > draw_dimensions.ymax)
            draw_dimensions.ymax = c_dims.ymax;
    }
}