#pragma once

#include "../DisplayInterface.hpp"

namespace graphics
{

    void scanline_rasterization(const ArrayView<Vec2i32>& points, DisplayInterface* iface, uint32_t color);

}