#include <interactive-ui/components/FunctionComponent.h>

void FunctionComponent::RedoFunctionLUT()
{
    const int32_t length = inversed ? draw_dimensions.Height() : draw_dimensions.Width();
    const float domain_length = domain.y - domain.x;
    function_value_lut.resize(length);
    extrema.x = +__FLT_MAX__;
    extrema.y = -__FLT_MAX__;
    for (int32_t i = 0; i < length; i++)
    {
        float v = func_raw((i / float(length - 1)) * (float)domain_length + domain.x);
        function_value_lut[i] = v;

        extrema.x = std::min(v, extrema.x);
        extrema.y = std::min(v, extrema.y);

    }
    AutoFit();
}

void FunctionComponent::DrawThickness(size_t i)
{
    float v_prev = function_value_lut[std::max(i - 1, 0U)];
    float v_curr = function_value_lut[i];
    float v_next = function_value_lut[std::min(i + 1, function_value_lut.size() - 1)];
    float deriv = (v_next - v_prev) * 0.5f * amplitude;

    Vec2f tan = inversed ? Vec2f{deriv, 1.f} : Vec2f{1.f, deriv};
    tan = tan.Normalize();

    int32_t half = thickness >> 1; // epic freaking optimization!!!!!!!!

    // when not inversed, it is negated for proper screen origin drawing
    Vec2f on_curve = inversed ? Vec2f{v_curr * amplitude, (float)i} : Vec2f{(float)i, -v_curr * amplitude};
    Vec2f norm = {-tan.y, tan.x}; // definition for 2d vector normal

    for (int32_t j = -half; j <= half; j++)
    {
        Vec2f pixel_f = on_curve + norm * (float)j;
        display->DrawPixel(origin_position + draw_dimensions.min + Vec2i32{(int32_t)std::round(pixel_f.x), (int32_t)std::round(pixel_f.y)}, color);
    }
}

FunctionComponent::FunctionComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& dimensions, MathFunction<float> func_raw, int32_t z_layer, Screen* initial_screen)
    : SelectableComponent(manager, origin, z_layer, initial_screen), func_raw(func_raw), domain(0.f, 1.f), function_value_lut(dimensions.x, 0.f)
{
    draw_dimensions.max = dimensions;
    RedoFunctionLUT();
}

void FunctionComponent::AutoFit()
{
    amplitude = draw_dimensions.Height() / (extrema.y - extrema.x);
}

void FunctionComponent::Draw()
{
    if (inversed)
        for (int32_t y = draw_dimensions.Height() - 1; y >= 0; y--)
            DrawThickness(y);
    else
        for (int32_t x = 0; x < draw_dimensions.Width(); x++)
            DrawThickness(x);
}

void FunctionComponent::Scale(const Vec2f& scale_vec)
{
    SelectableComponent::Scale(scale_vec);
    amplitude *= scale_vec.y;
    RedoFunctionLUT();
}

void FunctionComponent::SetDrawDimensions(const AABBi32& dims)
{
    SelectableComponent::SetDrawDimensions(dims);
    RedoFunctionLUT();
}