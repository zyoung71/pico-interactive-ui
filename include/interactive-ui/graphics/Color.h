#pragma once

#include <math/Vec3.h>
#include <math/Vec4.h>

namespace graphics
{
    union HSV;

    union RGBA
    {
        Vec4u8 vrgba;
        Vec3u8 vrgb;
        uint32_t rgba;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t alpha;

        constexpr inline operator uint32_t() const
        {
            return rgba;
        }

        constexpr inline RGBA() : rgba(0) {}
        constexpr inline RGBA(uint32_t rgba) : rgba(rgba) {}
        constexpr inline RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : RGBA((r << 24) | (g << 16) | (b << 8) | (a << 0)) {}
        constexpr inline RGBA(uint8_t r, uint8_t g, uint8_t b) : RGBA(r, g, b, 255) {}
        constexpr inline RGBA(Vec4u8 v) : RGBA(v.x, v.y, v.z, v.w) {}
        constexpr inline RGBA(Vec3u8 v) : RGBA(v.x, v.y, v.z, 255) {}

        constexpr HSV ToHSV() const;

        constexpr inline RGBA Inverted() const
        {
            return RGBA(Vec3u8(255) - vrgb);
        }
    };

    union HSV
    {
        Vec3f v;
        float hue;
        float saturation;
        float brightness;

        constexpr static inline HSV HSV_unchecked(Vec3f v)
        {
            HSV hsv;
            hsv.hue = v.x;
            hsv.saturation = v.y;
            hsv.brightness = v.z;
            return hsv;
        }

        constexpr inline operator Vec3f() const
        {
            return v;
        }

        constexpr inline HSV() : v(0.f, 0.f, 0.f) {}
        constexpr inline HSV(float hue, float saturation, float brightness) : v(std::fmodf(hue, 360.f), std::fmodf(saturation, 360.f), std::fmodf(brightness, 360.f)) {}
        constexpr inline HSV(Vec3f vhsv) : v(vhsv.x, vhsv.y, vhsv.z) {}

        constexpr RGBA ToRGB() const;

        constexpr inline HSV& ShiftHue(float degrees)
        {
            hue += degrees;
            hue = std::fmodf(hue, 360.f);
            return *this;
        }
    };
    
    constexpr HSV RGBA::ToHSV() const
    {
        Vec3f rgbf{(float)red, (float)green, (float)blue};
        HSV hsv;
        rgbf /= 255.f;
    
        float min, max, delta;
        
        min = rgbf.Min();
        max = rgbf.Max();
        delta = max - min;
        hsv.brightness = max;
    
        if (delta <= 0.f)
        {
            hsv.hue = 0.f;
            hsv.saturation = 0.f;
            return hsv;
        }
        if (max > 0.f)
            hsv.saturation = delta / max;
        else
        {
            hsv.hue = NAN;
            hsv.saturation = 0.f;
            return hsv;
        }
    
        if (max >= rgbf.x)
            hsv.hue = (rgbf.y - rgbf.z) / delta;
        else if (max >= rgbf.y)
            hsv.hue = ((rgbf.z - rgbf.x) / delta) + 2;
        else
            hsv.hue = ((rgbf.x - rgbf.y) / delta) + 4;
    
        hsv.hue *= 60.f;
    
        if (hsv.hue < 0.f)
            hsv.hue += 360.f;
    
        return hsv;
    }
    
    constexpr RGBA HSV::ToRGB() const
    {
        float hh, p, q, t, ff;

        int32_t i;
        RGBA rgba;
        rgba.alpha = 255;

        if (saturation <= 0.f)
        {
            rgba.vrgb = Vec3u8(brightness);
            return rgba;
        }

        hh = hue;
        if (hh >= 360.0f)
            hh = 0.f;

        hh /= 60.f;
        i = (int32_t)hh;
        ff = hh - i;
        p = brightness * (1.f - saturation);
        q = brightness * (1.f - (saturation * ff));
        t = brightness * (1.f - (saturation * (1.f - ff)));

        switch (i)
        {
            case 0:
                rgba.red = brightness;
                rgba.green = t;
                rgba.blue = p;
                break;
            case 1:
                rgba.red = q;
                rgba.green = brightness;
                rgba.blue = p;
                break;
            case 2:
                rgba.red = p;
                rgba.green = brightness;
                rgba.blue = t;
                break;

            case 3:
                rgba.red = p;
                rgba.green = q;
                rgba.blue = brightness;
                break;
            case 4:
                rgba.red = t;
                rgba.green = p;
                rgba.blue = brightness;
                break;
            case 5:
            default:
                rgba.red = brightness;
                rgba.green = p;
                rgba.blue = q;
                break;
        }

        return rgba;
    }

    namespace colors
    {
        constexpr RGBA CLEAR = 0x00000000;

        constexpr RGBA WHITE = 0xFFFFFFFF;
        constexpr RGBA BLACK = 0xFFFFFF00;
        constexpr RGBA GRAY = 0x7F7F7FFF;

        constexpr RGBA RED = 0xFF0000FF;
        constexpr RGBA ORANGE = 0xFF7F00FF;
        constexpr RGBA YELLOW = 0xFFFF00FF;
        constexpr RGBA LIME = 0x7FFF00FF;
        constexpr RGBA GREEN = 0x00FF00FF;
        constexpr RGBA SPRING_GREEN = 0x00FF7FFF;
        constexpr RGBA CYAN = 0x00FFFFFF;
        constexpr RGBA AZURE_BLUE = 0x007FFFFF;
        constexpr RGBA BLUE = 0x0000FFFF;
        constexpr RGBA INDIGO = 0x7F00FFFF;
        constexpr RGBA MAGENTA = 0xFF00FFFF;
        constexpr RGBA HOT_PINK = 0xFF007FFF;

        constexpr RGBA LIGHT_YELLOW = 0xFFFF7FFF;
        constexpr RGBA PINK = 0xFF7FFFFF;
        constexpr RGBA LIGHT_BLUE = 0x7FFFFFFF;
    }
}