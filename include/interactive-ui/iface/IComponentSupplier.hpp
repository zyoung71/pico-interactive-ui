#pragma once

class Component;

class IComponentSupplier
{
public:
    virtual Component* GetComponent() const = 0;
};