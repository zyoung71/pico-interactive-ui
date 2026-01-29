#include <interactive-ui/Component.h>
#include <cstring>

MovementAnimation::MovementAnimation(const Component* component, const EasingFunctionLUT& easing_func)
    : start_pos(component->GetOriginPosition()), easing_func(easing_func)
{
}

MovementAnimation::MovementAnimation()
    : start_pos(0, 0), easing_func(graphics::easing::lut_sine_in_out)
{
}

Component::Component(ScreenManager* manager, const Vec2i32& position, int32_t z_layer, const Screen* initial_screen, bool selectable)
    : manager(manager), display(manager->GetDisplay()), z_layer(z_layer), color(0xFFFFFFFF), selectable(selectable), forced_visibility(false), personal_visibility(true)
{
    if (initial_screen)
    {
        Vec2i32 screen_dim = initial_screen->GetDimensions();
        origin_position.x = position.x > screen_dim.x ? screen_dim.x : position.x;   
        origin_position.y = position.y > screen_dim.y ? screen_dim.y : position.y;
    }
    else
        origin_position = position;

    queue_init(&moving_queue, sizeof(MovementAnimation), moving_queue_size);
}

Component::Component(ScreenManager* manager, float x_percentage, float y_percentage, int32_t z_layer, const Screen* initial_screen, bool selectable)
    : Component(manager, {
        static_cast<int32_t>(initial_screen->GetDimensions().x * x_percentage),
        static_cast<int32_t>(initial_screen->GetDimensions().y * y_percentage)
    }, z_layer, initial_screen, selectable)
{
}

Component::~Component()
{
    queue_free(&moving_queue);
}

void Component::Update(float dt)
{
    MovementAnimation animation;
    MovementAnimation* ani_arr[moving_queue_size]; // This is here to avoid recursion and allow multiple movements to be queued again
    memset(ani_arr, 0, sizeof(ani_arr));
    uint8_t idx = 0;

    // no single animation will be found in the same queue twice. if only one animation
    // is in progress, the queue will stay at a size of one until the animation is complete
    while (queue_try_remove(&moving_queue, &animation))
    {
        if (!animation.moving)
            continue;

        if (animation.elapsed <= 0.f)
        {
            if (animation.on_animation_begin && animation.enable_callbacks)
                animation.on_animation_begin();
        }

        float k, eased;

        animation.elapsed += dt;
        
        k = animation.elapsed / animation.duration; // time ratio
        if (animation.reversed)
        {
            k = 1.f - k;
            if (k <= 0.f)
            {
                k = 0.f;
                animation.moving = false;
                manager->component_moving_reference_count--;
                if (animation.on_animation_end && animation.enable_callbacks)
                    animation.on_animation_end();
                
                origin_position = animation.start_pos;
                continue;
            }
        }
        else
        {
            if (k >= 1.f)
            {
                k = 1.f;
                animation.moving = false;
                manager->component_moving_reference_count--;
                if (animation.on_animation_end && animation.enable_callbacks)
                    animation.on_animation_end();

                origin_position = animation.end_pos;
                continue;
            }
        }
        ani_arr[idx++] = &animation; // add to array for later use

        size_t lut_idx = static_cast<size_t>(k * (graphics::easing::lut_size - 1));
        eased = animation.easing_func[lut_idx];

        // do floating point arithmetic to allow ratio results, then translate back to pixel coordinates
        origin_position = animation.start_pos + (Vec2f)animation.GetDelta() * eased;
    }

    for (uint8_t i = 0; i < moving_queue_size; i++)
    {
        MovementAnimation* ani = ani_arr[i];
        if (!ani)
            break; // no need to check the other indecies as a nullptr guarantees the rest are as well
        if (ani->moving)
            queue_try_add(&moving_queue, ani); // if not done, add to queue again. must be done outside the while loop
    }

    if (forced_visibility && personal_visibility)
        Draw(); // draw last
}

bool Component::Move(MovementAnimation animation, bool reversed, bool enable_callbacks)
{
    animation.reversed = reversed;
    animation.enable_callbacks = enable_callbacks;
    manager->component_moving_reference_count++;
    manager->UpdateDeltaTime();
    return queue_try_add(&moving_queue, &animation);
}

bool Component::IsMoving() const
{
    return !queue_is_empty(&moving_queue);
}