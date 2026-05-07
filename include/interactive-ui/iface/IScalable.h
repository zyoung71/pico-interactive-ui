#pragma once

template<typename D>
class IScalable
{
public:
    virtual D GetScale() const = 0;
    virtual void SetScale(D scale) = 0;
};