#include <interactive-ui/Component.h>
#include <cstring>

Component::Component(const ScreenManager* manager, const Vec2u32& position, int32_t z_layer, const Screen* initial_screen, bool selectable)
    : manager(manager), display(manager->GetDisplay()), z_layer(z_layer), color(0xFFFFFFFF), selectable(selectable), forced_visibility(false), personal_visibility(true)
{
    if (initial_screen)
    {
        Vec2u32 screen_dim = initial_screen->GetDimensions();
        origin_position.x = position.x > screen_dim.x ? screen_dim.x : position.x;   
        origin_position.y = position.y > screen_dim.y ? screen_dim.y : position.y;
    }
    else
        origin_position = position;

    queue_init(&moving_queue, sizeof(MovementAnimation), 4);
}

Component::Component(const ScreenManager* manager, float x_percentage, float y_percentage, int32_t z_layer, const Screen* initial_screen, bool selectable)
    : Component(manager, {
        static_cast<uint32_t>(initial_screen->GetDimensions().x * x_percentage),
        static_cast<uint32_t>(initial_screen->GetDimensions().y * y_percentage)
    }, z_layer, initial_screen, selectable)
{
}

Component::~Component()
{
    queue_free(&moving_queue);
}

void Component::Update(float dt)
{
    MovementAnimation* animation;
    MovementAnimation* ani_arr[4]; // This is here to avoid recursion and allow multiple movements to be queued again
    memset(ani_arr, 0, sizeof(MovementAnimation*) * 4);
    uint8_t idx;
    while (queue_try_remove(&moving_queue, animation))
    {
        if (!animation->moving)
            continue;

        float k, eased;

        if (!animation->pos_set_latch)
        {
            animation->pos_set_latch = true;
            animation->start_pos = origin_position;
        }

        animation->elapsed += dt;
        
        k = animation->elapsed / animation->duration; // time ratio
        if (k >= 1.f)
        {
            k = 1.f;
            animation->moving = false;
        }
        else
            ani_arr[idx++] = animation;

        size_t lut_idx = static_cast<size_t>(k * (graphics::easing::lut_size - 1));
        eased = animation->easing_func[lut_idx];

        // do floating point arithmetic to allow ratio results, then translate back to pixel coordinates
        origin_position += static_cast<Vec2u32>(((Vec2f)animation->end_pos) * k);
    }

    moving = false;
    for (uint8_t i = 0; i < 4; i++)
    {
        MovementAnimation* ani = ani_arr[i];
        if (ani->moving)
        {
            queue_try_add(&moving_queue, &ani); // if not done, add to queue again
            moving = true;
        }
    }

    if (forced_visibility && personal_visibility)
        Draw(); // draw last
}

bool Component::Move(const MovementAnimation* animation)
{
    return queue_try_add(&moving_queue, animation);
}