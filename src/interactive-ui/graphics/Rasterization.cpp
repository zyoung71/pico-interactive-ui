#include <interactive-ui/graphics/Rasterization.h>
#include <utility>

namespace graphics
{

    void scanline_rasterization(const ArrayView<Vec2i32>& points, DisplayInterface* iface, uint32_t color)
    {
        if (points.length < 3)
            return; // Must be at least a triangle.

        float ymin = points[0].y;
        float ymax = ymin;

        for (size_t i = 1; i < points.length; i++)
        {
            float y = points[i].y;
            ymin = std::min(ymin, y);
            ymax = std::max(ymax, y);
        }

        float intersections[points.length];
        size_t intersect_count;
        for (int32_t y = (int32_t)ymin; y <= (int32_t)ymax; y++)
        {
            intersect_count = 0;

            for (size_t i = 0; i < points.length; i++)
            {
                float x1 = points[i].x;
                float x2 = points[(i - 1) % points.length].x;
                float y1 = points[i].y;
                float y2 = points[(i + 1) % points.length].y;
                
                if ((y1 <= y && y2 > y) || (y2 <= y && y1 > y)) 
                {
                    float interp = (y - y1) / (y2 - y1);
                    intersections[intersect_count++] = interp * (x2 - x1) + x1;
                }
            }

            for (size_t i = 0; i < intersect_count; i++)
            {
                for (size_t j = i + 1; j < intersect_count; j++)
                {
                    if (intersections[j] < intersections[i])
                        std::swap(intersections[i], intersections[j]);
                }
            }

            for (size_t i = 0; (i + 1) < intersect_count; i += 2)
            {
                int32_t x0 = intersections[i];
                int32_t x1 = intersections[i + 1];

                for (int32_t x = x0; x <= x1; x++)
                    iface->DrawPixel({x, y}, color);
            }
        }
    }

}