#pragma once

#include <vector>
#include <math/Vec2.h>
#include "IComponentSupplier.h"

struct CollisionManifold
{
    Vec2f normal;
    float penetration;
};

class PhysicsMixin : public IComponentSupplier
{
private:
    static std::vector<PhysicsMixin*> physics_objects;

protected:
    Vec2f physics_position;
    Vec2f physics_velocity;

    float restitution;

private:
    float mass;
    float inv_mass; // 1 / mass

public:
    void Update(float dt);

    virtual bool CollidesWith(const PhysicsMixin& other, CollisionManifold& cm) = 0;

    inline void SetMass(float m)
    {
        mass = m;
        inv_mass = 1.f / m;
    }
    inline float GetMass() const
    {
        return mass;
    }
};