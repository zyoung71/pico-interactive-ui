#include "Component.h"

class SelectableComponent : public Component
{
protected:
    bool is_selected;

public:
    union
    {
        SelectableComponent* neighboring_components[4];
        struct
        {
            SelectableComponent* up_component;
            SelectableComponent* down_component;
            SelectableComponent* left_component;
            SelectableComponent* right_component;
        };
    };

public:
    SelectableComponent(Screen* screen, const Vec2u32& position);
    SelectableComponent(Screen* screen, float x_percentage, float y_percentage);
    virtual ~SelectableComponent() = default;

    inline void SetSelected(bool is_selected)
    {
        this->is_selected = is_selected;
    }

    inline bool IsSelected() const
    {
        return is_selected;
    }
};