#pragma once

#include "../SelectableComponent.h"
#include "../Pixel.h"

/**
 * Colors in specific pixels based on the vector positions given. Unlike BitmapComponent,
 * it will ONLY set the pixels based on the vector positions given.
 * Initially inside the constructor, you can pass an array of positions and their colors.
 */
class PixelBufferComponent : public SelectableComponent
{
protected:
    std::unordered_map<Vec2i32, uint32_t> pixels;

public:
    PixelBufferComponent(ScreenManager* manager, const Vec2i32& origin, const Vec2i32& dimensions, int32_t z_layer, Screen* initial_screen = nullptr);
    PixelBufferComponent(ScreenManager* manager, const Vec2f& screen_percentage, const Vec2i32& dimensions, int32_t z_layer, Screen* initial_screen);
    PixelBufferComponent(ScreenManager* manager, const Vec2i32& origin, const Pixel* initial_buffer, size_t buff_length, int32_t z_layer, Screen* initial_screen = nullptr);
    PixelBufferComponent(ScreenManager* manager, const Vec2i32& origin, const ArrayView<Pixel>& initial_buffer, int32_t z_layer, Screen* initial_screen = nullptr);
    PixelBufferComponent(ScreenManager* manager, const Vec2f& screen_percentage, const Pixel* initial_buffer, size_t buff_length, Screen* initial_screen);
    PixelBufferComponent(ScreenManager* manager, const Vec2f& screen_percentage, const ArrayView<Pixel>& initial_buffer, int32_t z_layer, Screen* initial_screen);

    virtual ~PixelBufferComponent() = default;

    void SetPixel(Pixel p);

    void Draw() override;
};