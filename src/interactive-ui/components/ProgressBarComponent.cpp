#include <interactive-ui/components/ProgressBarComponent.h>

#include <cstring>

ProgressBarComponent::ProgressBarComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& box_dimensions, const Font* font, int32_t z_layer, Screen* initial_screen)
    : TextBoxComponent(manager, origin, box_dimensions, nullptr, font, z_layer, initial_screen)
{
    text = new char[5 + decimal_places];
}

ProgressBarComponent::~ProgressBarComponent()
{
    delete[] text;
}

void ProgressBarComponent::SetDecimalPlaces(size_t decimal_places)
{
    if (this->decimal_places == decimal_places)
        return;

    this->decimal_places = decimal_places;
    if (decimal_places > 0)
        decimal_places++; // adding the dot

    char* moved = new char[5 + decimal_places];
    strcpy(moved, text);
    
    delete[] text;
    text = moved;

    Align();
}