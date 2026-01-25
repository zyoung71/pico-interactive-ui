#pragma once

#include "../SelectableComponent.h"

#include <map>

struct Pixel
{
    Vec2u32 position;
    uint32_t color;
};

/**
 * Colors in specific pixels based on the vector positions given. Unlike BitmapComponent,
 * it will ONLY set the pixels based on the vector positions given.
 * Initially inside the constructor, you can pass an array of positions and their colors.
 */
class PixelBufferComponent : public SelectableComponent
{
protected:
    std::map<Vec2u32, uint32_t> pixels;

public:
    PixelBufferComponent(const ScreenManager* manager, const Vec2u32& origin, const Vec2u32& dimensions, int32_t z_layer, const Screen* initial_screen = nullptr);
    PixelBufferComponent(const ScreenManager* manager, float x_percentage, float y_percentage, const Vec2u32& dimensions, int32_t z_layer, const Screen* initial_screen);
    PixelBufferComponent(const ScreenManager* manager, const Vec2u32& origin, const Pixel* initial_buffer, size_t buff_length, int32_t z_layer, const Screen* initial_screen = nullptr);
    PixelBufferComponent(const ScreenManager* manager, const Vec2u32& origin, const ArrayView<Pixel>& initial_buffer, int32_t z_layer, const Screen* initial_screen = nullptr);
    PixelBufferComponent(const ScreenManager* manager, float x_percentage, float y_percentage, const Pixel* initial_buffer, size_t buff_length, const Screen* initial_screen);
    PixelBufferComponent(const ScreenManager* manager, float x_percentage, float y_percentage, const ArrayView<Pixel>& initial_buffer, int32_t z_layer, const Screen* initial_screen);

    virtual ~PixelBufferComponent() = default;

    void SetPixel(Pixel p);

    void Draw() override;
};