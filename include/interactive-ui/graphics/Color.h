#pragma once

#include <math/Vec3.h>
#include <math/Vec4.h>

namespace graphics
{

    constexpr uint32_t rgba(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
    {
        return (red << 24) | (green << 16) | (blue << 8) | (alpha << 0);
    }
    constexpr uint32_t rgba(Vec4u8 vrgba)
    {
        return rgba(vrgba.x, vrgba.y, vrgba.z, vrgba.w);
    }
    constexpr uint32_t rgb(Vec3u8 vrgb)
    {
        return rgba(vrgb.x, vrgb.y, vrgb.z, 255);
    }
    constexpr uint32_t rgb(uint8_t red, uint8_t green, uint8_t blue)
    {
        return rgba(red, green, blue, 255);
    }
    
    constexpr Vec3f hsv(float hue, float saturation, float brightness)
    {
        return Vec3f{std::fmodf(hue, 360.f), std::fmodf(saturation, 360.f), std::fmodf(brightness, 360.f)};
    }
    constexpr Vec3f hsv_unchecked(float hue, float saturation, float brightness)
    {
        return Vec3f{hue, saturation, brightness};
    }
    
    constexpr Vec3f rgb_to_hsv(uint8_t red, uint8_t green, uint8_t blue)
    {
        Vec3f rgbf{(float)red, (float)green, (float)blue};
        Vec3f hsv;
        rgbf /= 255.f;
    
        float min, max, delta;
        
        min = rgbf.Min();
        max = rgbf.Max();
        delta = max - min;
        hsv.z = max;
    
        if (delta <= 0.f)
        {
            hsv.x = 0.f;
            hsv.y = 0.f;
            return hsv;
        }
        if (max > 0.f)
            hsv.y = delta / max;
        else
        {
            hsv.x = NAN;
            hsv.y = 0.f;
            return hsv;
        }
    
        if (max >= rgbf.x)
            hsv.x = (rgbf.y - rgbf.z) / delta;
        else if (max >= rgbf.y)
            hsv.x = ((rgbf.z - rgbf.x) / delta) + 2;
        else
            hsv.x = ((rgbf.x - rgbf.y) / delta) + 4;
    
        hsv.x *= 60.f;
    
        if (hsv.x < 0.f)
            hsv.x += 360.f;
    
        return hsv;
    }
    constexpr Vec3f rgb_to_hsv(Vec3u8 vrgb)
    {
        return rgb_to_hsv(vrgb.x, vrgb.y, vrgb.z);
    }
    
    constexpr uint32_t hsv_to_rgb(float hue, float saturation, float brightness)
    {
        // todo
        return 0;
    }
    constexpr uint32_t hsv_to_rgb(const Vec3f& hsv)
    {
        return hsv_to_rgb(hsv.x, hsv.y, hsv.z);
    }

    constexpr void hsv_hue_shift(float& hue, float degrees)
    {
        hue += degrees;
        hue = std::fmodf(hue, 360.f);
    }
    constexpr void hsv_hue_shift(Vec3f& hsv, float degrees)
    {
        hsv_hue_shift(hsv.x, degrees);
    }

}