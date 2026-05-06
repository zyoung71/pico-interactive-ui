#include <interactive-ui/iface/PhysicsMixin.h>

void PhysicsMixin::Update(float dt)
{
    CollisionManifold cm;
    for (size_t i = 0; i < physics_objects.size(); i++)
    {
        PhysicsMixin* other = physics_objects[i];
        if (this == other)
            continue;

        if (CollidesWith(*other, cm))
        {
            Vec2f relative_velocity = other->physics_velocity - physics_velocity;
            float velocity_along_normal = relative_velocity.Dot(cm.normal);

            if (velocity_along_normal > 0)
                continue;

            float impulse_mag = -(1.f - restitution) * velocity_along_normal;
            Vec2f impulse = cm.normal * impulse_mag;

            physics_velocity -= impulse * inv_mass;
            
            constexpr float correction_strength = 0.80f;
            constexpr float allowed_penetration = 0.01f;
            Vec2f correction = cm.normal * (correction_strength * (std::max(cm.penetration - allowed_penetration, 0.f) / (inv_mass + other->inv_mass)));
            physics_position -= correction * inv_mass;
        }
    }

    physics_position += physics_velocity * dt;
}