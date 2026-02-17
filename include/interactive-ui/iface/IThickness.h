#pragma once

class Component;

template<typename D>
class IThickness
{
public:
    virtual D GetThickness() const = 0;
    virtual void SetThickness(D thickness) = 0;
    virtual Component* GetComponent() const = 0;
};