#include "Component.h"

#include <util/ResultOptional.h>
#include <util/ArrayView.h>

class SelectableComponent : public Component
{
protected:
    bool is_selected;

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
    SelectableComponent();
    virtual ~SelectableComponent();

    inline void SetSelected(bool is_selected)
    {
        this->is_selected = is_selected;
    }

    inline bool IsSelected() const
    {
        return is_selected;
    }

    inline constexpr ArrayView<SelectableComponent*> GetNeighboringComponents()
    {
        return make_array_view(neighboring_components);
    }

    inline ResultOptional<SelectableComponent*, bool> GetUpComponent()
    {
        if (up_component != nullptr)
            return ResultOptional(up_component, true);
        
        return ResultOptional(this, false);
    }

    inline ResultOptional<SelectableComponent*, bool> GetDownComponent()
    {
        if (down_component != nullptr)
            return ResultOptional(down_component, true);
        
        return ResultOptional(this, false);
    }

    inline ResultOptional<SelectableComponent*, bool> GetLeftComponent()
    {
        if (left_component != nullptr)
            return ResultOptional(left_component, true);
        
        return ResultOptional(this, false);
    }

    inline ResultOptional<SelectableComponent*, bool> GetRightComponent()
    {
        if (right_component != nullptr)
            return ResultOptional(right_component, true);
        
        return ResultOptional(this, false);
    }
};