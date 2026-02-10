#include <interactive-ui/components/DropDownBoxComponent.h>

DropDownBoxComponent::DropDownBoxComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& main_box_dimensions, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen)
    : Component(manager, origin, z_layer, initial_screen),
    main_box(manager, origin, main_box_dimensions, text, font, z_layer, initial_screen),
    arrow_box(manager, origin + Vec2i32{main_box_dimensions.x, 0}, Vec2i32{7, main_box_dimensions.y}, z_layer, initial_screen),
    arrow(manager, arrow_box.GetOriginPosition() + main_box.padding, bitmap_dropdown_arrow, z_layer, initial_screen),
    options(manager, origin, z_layer, initial_screen)
{
    draw_dimensions.min = main_box.GetDrawDimensions().min;
    draw_dimensions.max = main_box.GetDrawDimensions().max + arrow_box.GetDrawDimensions().max;
}

void DropDownBoxComponent::Expand(bool down)
{
    expanded = down;
    draw_dimensions.ymax += options.GetDrawDimensions().ymax;
    options.SetPersonalVisibility(down);
    arrow.mirror_vertically = down;
}

void DropDownBoxComponent::Update(float dt, const Screen* screen)
{
    Component::Update(dt, screen);
    arrow_box.Update(dt, screen);
    arrow.Update(dt, screen);
    options.Update(dt, screen);
}

void DropDownBoxComponent::Draw()
{
    Component::Draw();
    arrow_box.Draw();
    arrow.Draw();
    options.Draw();
}

void DropDownBoxComponent::Align()
{
    Component::Align();
    arrow_box.Align();
    arrow.Align();
    options.Align();
}

void DropDownBoxComponent::Scale()
{
    Component::Scale();
    arrow_box.Scale();
    arrow.Scale();
    options.Scale();
}

void DropDownBoxComponent::OnExitScreen(const Screen* screen)
{
    if (expanded)
        Expand(false);
}

void DropDownBoxComponent::SetOriginPosition(const Vec2i32& pos)
{
    Component::SetOriginPosition(pos);
    arrow_box.SetOriginPosition(pos + main_box.GetDrawDimensions().Size());
    arrow.SetOriginPosition(pos + main_box.GetDrawDimensions().Size() + main_box.padding);
    options.SetOriginPosition(pos + Vec2i32{0, main_box.GetDrawDimensions().Size().y});
}

void DropDownBoxComponent::ForceVisibility(bool visibility)
{
    Component::ForceVisibility(visibility);
    arrow_box.ForceVisibility(visibility);
    arrow.ForceVisibility(visibility);
    if (!visibility)
        options.ForceVisibility(false);
}

void DropDownBoxComponent::SetPersonalVisibility(bool visibility)
{
    Component::SetPersonalVisibility(visibility);
    arrow_box.SetPersonalVisibility(visibility);
    arrow.SetPersonalVisibility(visibility);
    if (!visibility)
        options.SetPersonalVisibility(false);
}