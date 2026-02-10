#include <interactive-ui/Component.h>
#include <interactive-ui/components/PaddingComponent.h>
#include <cstring>

MovementAnimation::MovementAnimation(const Component* component, const EasingFunctionLUT& easing_func)
    : start_pos(component->origin_position), easing_func(easing_func), start_scale(component->draw_dimensions)
{
}

MovementAnimation::MovementAnimation()
    : start_pos(0, 0), easing_func(graphics::easing::lut_sine_in_out), start_scale(0, 0, 0, 0)
{
}

void Component::MoveRefInc()
{
    manager->master_component_moving_reference_count++;
    for (auto s : screen_set)
        s->component_moving_reference_count++;
}

void Component::MoveRefDec()
{
    manager->master_component_moving_reference_count--;
    for (auto s : screen_set)
        s->component_moving_reference_count--;
}

Component::Component(ScreenManager* manager, const Vec2i32& position, int32_t z_layer, Screen* initial_screen, bool selectable)
    : origin_position(position), manager(manager), display(manager->GetDisplay()), z_layer(z_layer), color(0xFFFFFFFF),
    selectable(selectable), forced_visibility(false), personal_visibility(true), cancel_movements_flag(false)
{
    if (initial_screen)
        initial_screen->AddComponent(this);

    queue_init(&moving_queue, sizeof(MovementAnimation), moving_queue_size);
}

Component::Component(ScreenManager* manager, const Vec2f& screen_percentage, int32_t z_layer, Screen* initial_screen, bool selectable)
    : Component(manager, Vec2i32{
        static_cast<int32_t>(initial_screen->GetDimensions().x * screen_percentage.x),
        static_cast<int32_t>(initial_screen->GetDimensions().y * screen_percentage.y)
    }, z_layer, initial_screen, selectable)
{
}

Component::Component(const Component& to_copy)
    : origin_position(to_copy.origin_position), manager(to_copy.manager), display(to_copy.display),
    z_layer(to_copy.z_layer), color(to_copy.color), selectable(to_copy.selectable), forced_visibility(to_copy.forced_visibility),
    personal_visibility(to_copy.personal_visibility), screen_set(to_copy.screen_set), draw_dimensions(to_copy.draw_dimensions),
    cancel_movements_flag(to_copy.cancel_movements_flag)
{
    for (auto s : screen_set)
        s->AddComponent(this);

    queue_init(&moving_queue, sizeof(MovementAnimation), moving_queue_size);
}

Component::~Component()
{
    queue_free(&moving_queue);
}

void Component::Update(float dt, const Screen* screen)
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

        if (cancel_movements_flag)
        {
            animation.moving = false;
            MoveRefDec();
            
            if (animation.on_animation_end && animation.enable_callbacks)
                animation.on_animation_end(&animation);

            continue;
        }

        if (animation.elapsed <= 0.f)
        {
            if (animation.on_animation_begin && animation.enable_callbacks)
                animation.on_animation_begin(&animation);
        }

        float k, eased, scaled;

        animation.elapsed += dt;
        k = animation.elapsed / animation.duration; // time ratio

        // i know there's boilerplate here but minimalizing it won't optimise performance
        if (animation.reversed)
        {
            k = 1.f - k;
            if (k <= 0.f)
            {
                // initial animation finish
                switch (animation.type)
                {
                    case MovementAnimation::Type::NORMAL : {
                        animation.moving = false;
                        MoveRefDec();
                        if (animation.on_animation_end && animation.enable_callbacks)
                            animation.on_animation_end(&animation);
                        
                        if (animation.transpose)
                        {
                            origin_position = animation.start_pos;
                            if ((Component*)screen->hovered_component == this)
                                screen->hover_design->SetOriginPosition(animation.start_pos);
                        }
                        if (animation.scale)
                        {
                            draw_dimensions = animation.end_scale;
                            if ((Component*)screen->hovered_component == this)
                                screen->hover_design->draw_dimensions = animation.start_scale;   
                        }
                        continue;
                    };
                    case MovementAnimation::Type::ENDLESS : {
                        animation.elapsed = 0.f;
                        break;
                    };
                    case MovementAnimation::Type::ENDLESS_WITH_REVERSION : {
                        animation.elapsed = 0.f;
                        animation.reversed = !animation.reversed;
                        break;
                    };
                }
            }
        }
        else
        {
            if (k >= 1.f)
            {
                switch (animation.type)
                {
                    case MovementAnimation::Type::NORMAL : {
                        animation.moving = false;
                        MoveRefDec();
                        if (animation.on_animation_end && animation.enable_callbacks)
                            animation.on_animation_end(&animation);
                        
                        if (animation.transpose)
                        {
                            origin_position = animation.end_pos;
                            if ((Component*)screen->hovered_component == this)
                                screen->hover_design->SetOriginPosition(animation.end_pos);
                        }
                        if (animation.scale)
                        {
                            draw_dimensions = animation.end_scale;
                            if ((Component*)screen->hovered_component == this)
                                screen->hover_design->draw_dimensions = animation.end_scale;   
                        }
                        
                        continue;
                    };
                    case MovementAnimation::Type::ENDLESS : {
                        animation.elapsed = 0.f;
                        break;
                    };
                    case MovementAnimation::Type::ENDLESS_WITH_REVERSION : {
                        animation.elapsed = 0.f;
                        animation.reversed = !animation.reversed;
                        break;
                    };
                }
            }
        }
        // if an animation is done playing for good, the code below should NOT be ran

        ani_arr[idx++] = &animation; // add to array for later use

        size_t lut_idx = static_cast<size_t>(k * (graphics::easing::lut_size - 1));
        eased = animation.easing_func[lut_idx];

        // do floating point arithmetic to allow ratio results, then translate back to pixel coordinates
        if (animation.transpose)
        {
            Vec2i32 pos = animation.start_pos + (Vec2f)animation.GetTransposeDelta() * eased;
            origin_position = pos;
            if ((Component*)screen->hovered_component == this)
                screen->hover_design->SetOriginPosition(pos);
        }
        if (animation.scale)
        {
            Vec4i32 scale = animation.start_scale.vec + (Vec4f)animation.GetScaleDelta() * eased;
            draw_dimensions.vec = scale;
            if ((Component*)screen->hovered_component == this)
                screen->hover_design->draw_dimensions = scale;
        }
    }

    if (cancel_movements_flag)
        cancel_movements_flag = false;

    for (uint8_t i = 0; i < moving_queue_size; i++)
    {
        MovementAnimation* ani = ani_arr[i];
        if (!ani)
            break; // no need to check the other indecies as a nullptr guarantees the rest are as well
        if (ani->moving)
            queue_try_add(&moving_queue, ani); // if not done, add to queue again. must be done outside the while loop
    }

    if (forced_visibility && personal_visibility)
    {
        AABBi32 check_bounds(origin_position + draw_dimensions.min, origin_position + draw_dimensions.max);
        if (check_bounds.Intersects(screen->dimensions))
        {
            Draw(); // draw last
        }
    }
}

void Component::Align()
{
    Vec2i32 component_size = draw_dimensions.Size();

    switch (vertical_alignment)
    {
        case AlignmentVertical::TOP: {
            draw_dimensions.ymin = 0;
            draw_dimensions.ymax = component_size.y;
            break;
        }
        case AlignmentVertical::CENTER: {
            int32_t offset_y = component_size.y / 2;
            draw_dimensions.ymin = -offset_y;
            draw_dimensions.ymax = component_size.y - offset_y;
            break;
        }
        case AlignmentVertical::BOTTOM: {
            draw_dimensions.ymin = -component_size.y;
            draw_dimensions.ymax = 0;
            break;
        }
    }
    switch (horizontal_alignment)
    {
        case AlignmentHorizontal::LEFT: {
            draw_dimensions.xmin = 0;
            draw_dimensions.xmax = component_size.x;
            break;
        }
        case AlignmentHorizontal::CENTER: {
            int32_t offset_x = component_size.x / 2;
            draw_dimensions.xmin = -offset_x;
            draw_dimensions.xmax = component_size.x - offset_x;
            break;
        }
        case AlignmentHorizontal::RIGHT: {
            draw_dimensions.xmin = -component_size.x;
            draw_dimensions.xmax = 0;
            break;
        }
    }
}

void Component::SetOriginPosition(const Vec2i32& pos)
{
    origin_position = pos;
}

void Component::SetVerticalAlignment(AlignmentVertical align_v)
{
    vertical_alignment = align_v;
    Align();
}

void Component::SetHorizontalAlignment(AlignmentHorizontal align_h)
{
    horizontal_alignment = align_h;
    Align();
}

void Component::SetAlignment(AlignmentVertical align_v, AlignmentHorizontal align_h)
{
    vertical_alignment = align_v;
    horizontal_alignment = align_h;
    Align();
}

void Component::SetZLayer(int32_t layer)
{
    z_layer = layer;
    for (auto s : screen_set)
        s->SortComponents();
}

bool Component::Move(MovementAnimation animation, bool reversed, bool enable_callbacks)
{
    manager->UpdateDeltaTime();

    float dt = manager->last_dt;
    
    if (animation.duration <= 0.f)
    {
        origin_position = reversed ? animation.start_pos : animation.end_pos;
        if (enable_callbacks)
        {
            if (animation.on_animation_begin)
                animation.on_animation_begin(&animation);
            if (animation.on_animation_end)
                animation.on_animation_end(&animation);
        }
        
        for (auto s : screen_set)
        {
            if (s == manager->selected_screen)
            {
                manager->Update(dt);
                continue;
            }
            s->Update(dt);
        }
        return true;
    }

    animation.reversed = reversed;
    animation.enable_callbacks = enable_callbacks;

    MoveRefInc();
    
    return queue_try_add(&moving_queue, &animation);
}

bool Component::IsMoving() const
{
    return !queue_is_empty(&moving_queue);
}