#pragma once

#include <hardware/GPIODevice.h>

class Component;

class CursorMixin
{
public:
    float velocity = 10.f; // pixels per second
    union
    {
        struct
        {
            const GPIODevice* up_dev;
            const GPIODevice* down_dev;
            const GPIODevice* left_dev;
            const GPIODevice* right_dev;
        };
        const GPIODevice* directional_devices[4];
    };

public:
    inline CursorMixin() : CursorMixin(nullptr, nullptr, nullptr, nullptr) {}
    inline CursorMixin(const GPIODevice* up_dev, const GPIODevice* down_dev, const GPIODevice* left_dev, const GPIODevice* right_dev)
        : up_dev(up_dev), down_dev(down_dev), left_dev(left_dev), right_dev(right_dev) {}
    inline CursorMixin(const GPIODevice** devs) // must be of size 4
        : up_dev(devs[0]), down_dev(devs[1]), left_dev(devs[2]), right_dev(devs[3]) {}

    virtual ~CursorMixin() = default;

    virtual void OnHoverOverComponent(const Component* component) = 0;
    virtual void OnUnhoverOverComponent(const Component* component) = 0;
    virtual Component* GetComponent() const = 0;

    
};