#pragma once

#include "../ComponentPack.h"
#include "TextComponent.h"
#include "PaddingComponent.h"
#include "BitmapComponent.h"

constexpr uint32_t data_dropdown_arrow[4][7] = {
    {1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 0, 0, 0}
};

constexpr ArrayView2D<uint32_t> bitmap_dropdown_arrow = make_array_view(data_dropdown_arrow);

class DropDownBoxComponent : public Component
{
protected:
    TextBoxComponent main_box;
    PaddingComponent arrow_box;
    BitmapComponent arrow;

public:
    ComponentPack options;
    
private:
    bool expanded;

public:
    DropDownBoxComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& main_box_dimensions, const char* text, const Font* font, int32_t z_layer, Screen* initial_screen = nullptr);

    inline ComponentPack& GetOptionPack()
    {
        return options;
    }

    void Expand(bool down);
    inline void ExpandToggle()
    {
        expanded = !expanded;
        return Expand(expanded);
    }

    void Update(float dt, const Screen* screen);
    void Draw() override;
    void Align() override;
    void Scale(const Vec2f& scale_vec) override;

    void OnExitScreen(const Screen* screen) override;

    void SetOriginPosition(const Vec2i32& pos) override;
    void ForceVisibility(bool visibility) override;
    void SetPersonalVisibility(bool visibility) override;
};