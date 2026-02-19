#pragma once

#include "../SelectableComponent.h"
#include "../iface/IThickness.h"

class FunctionComponent : public SelectableComponent, public IThickness<uint32_t>
{
private:
    void RedoFunctionLUT();
    void DrawThickness(size_t i);

protected:
    std::vector<float> function_value_lut;
    Vec2f domain;
    Vec2f extrema;
    uint32_t thickness = 1;
    MathFunction<float> func_raw; // alternative option
    bool inversed = false;

public:
    float amplitude = 1.f;

public:
    FunctionComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& dimensions, MathFunction<float> func_raw, int32_t z_layer, Screen* initial_screen = nullptr);

    void Draw() override;
    void Scale(const Vec2f& scale_vec) override;

    void SetDrawDimensions(const AABBi32& dims) override;

    void AutoFit();

    inline void SetDomain(const Vec2f& domain)
    {
        this->domain = domain;
        RedoFunctionLUT();
    }
    inline void SetFunction(MathFunction<float> func)
    {
        func_raw = func;
        RedoFunctionLUT();
    }
    inline void SetInverse(bool inverse)
    {
        this->inversed = inverse;
        RedoFunctionLUT();
    }
    inline Vec2f GetDomain() const
    {
        return domain;
    }
    inline MathFunction<float> GetFunction() const
    {
        return func_raw;
    }
    inline bool IsInversed() const
    {
        return inversed;
    }

    inline uint32_t GetThickness() const override
    {
        return thickness;
    }
    inline void SetThickness(uint32_t thick) override
    {
        thickness = thick;
    }
    inline Component* GetComponent() const override
    {
        return (Component*)this;
    }
};