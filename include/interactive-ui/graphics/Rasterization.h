#pragma once

#include "../DisplayInterface.h"
#include <util/Types.h>

namespace graphics
{

    void scanline_rasterization(const ArrayView<Vec2i32>& points, DisplayInterface* iface, uint32_t color);

}