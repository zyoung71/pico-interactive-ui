#include <interactive-ui/ScreenManager.h>
#include <interactive-ui/Screen.h>
#include <interactive-ui/Component.h>

#include <interactive-ui/components/PixelBufferComponent.h>

static constexpr Pixel cursor_unhover_data[] = {
    {{2, 0}, 1},
    {{2, 1}, 1},
    {{0, 2}, 1},
    {{1, 2}, 1},
    {{3, 2}, 1},
    {{4, 2}, 1},
    {{2, 3}, 1},
    {{2, 4}, 1},
};

static constexpr Pixel cursor_hover_data[] = {
    {{0, 0}, 1},
    {{1, 0}, 1},
    {{3, 0}, 1},
    {{4, 0}, 1},
    {{0, 1}, 1},
    {{4, 1}, 1},
    {{0, 3}, 1},
    {{4, 3}, 1},
    {{0, 4}, 1},
    {{1, 4}, 1},
    {{3, 4}, 1},
    {{4, 4}, 1},
};

static constexpr ArrayView<Pixel> cursor_unhover = make_array_view(cursor_unhover_data);
static constexpr ArrayView<Pixel> cursor_hover = make_array_view(cursor_hover_data);

class DefaultCursorComponent : public PixelBufferComponent, public CursorMixin
{
public:
    DefaultCursorComponent(ScreenManager* manager)
        : PixelBufferComponent(manager, Vec2i32{-10, -10}, cursor_unhover, INT32_MAX, nullptr),
        CursorMixin() {}

    void OnHoverOverComponent(const Component* component) override
    {
        ClearPixels();
        SetPixels(cursor_hover);
    }
    void OnUnhoverOverComponent(const Component* component) override
    {
        ClearPixels();
        SetPixels(cursor_unhover);
    }
    inline Component* GetComponent() const
    {
        return (Component*)this;
    }
};

ScreenManager::ScreenManager(DisplayInterface* const display)
    : display(display), selected_screen(nullptr), cursor(std::make_unique<DefaultCursorComponent>(this))
{
    if (refresh_rate > 0.f)
        refresh_period = 1.f / refresh_rate;
    else
        refresh_period = 0.f;

    queue_init(&control_queue, sizeof(ControlAction), 4);
}

ScreenManager::~ScreenManager()
{
    queue_free(&control_queue);
}

void ScreenManager::PushScreen(Screen* screen)
{
    if (screens.size() == 0) // if this is the first screen being pushed on the stack
    {
        cursor->GetComponent()->SetOriginPosition(screen->GetDimensions() / 2); // centers the cursor
    }
    screens.push(screen);
    screen_set.insert(screen);
    screen->manager = this; // Should be set already.
    if (selected_screen)
        selected_screen->OnScreenDeselect();
    selected_screen = screen;
    selected_screen->OnScreenSelect();
    Vec2i32 pos = cursor->GetComponent()->GetOriginPosition();
    const AABBi32 dim = selected_screen->dimensions;
    if (pos.x < dim.xmin)
        pos.x = dim.xmin;
    else if (pos.x > dim.xmax)  
        pos.x = dim.xmax;
    
    if (pos.y < dim.ymin)
        pos.y = dim.ymin;
    else if (pos.y > dim.ymax)
        pos.y = dim.ymax;

    cursor->GetComponent()->SetOriginPosition(pos);
}

void ScreenManager::PopScreen()
{
    screens.pop();
    selected_screen->OnScreenDeselect();
    selected_screen = screens.top();
    selected_screen->OnScreenSelect();
}

void ScreenManager::EnableCursor(bool enable)
{
    enable_cursor = enable;
    cursor->GetComponent()->SetPersonalVisibility(enable);
}

void ScreenManager::EnableCBF(bool on)
{
    click_between_frames = on;
    while (queue_try_remove(&control_queue, nullptr)) {} // empty the queue
}

void ScreenManager::QueueControl(uint64_t action_mask)
{
    if (click_between_frames)
        return selected_screen->OnControl(action_mask);
    queue_try_add(&control_queue, &action_mask);
}

void ScreenManager::UpdateDeltaTime()
{
    static absolute_time_t then = 0;
    absolute_time_t now = to_us_since_boot(get_absolute_time());
    absolute_time_t dt_us = absolute_time_diff_us(then, now);
    last_dt = dt_us * 1e-6f;
    then = now;
}

static float cumulative_dt = 0.f;

void ScreenManager::Update()
{
    UpdateDeltaTime();
    cumulative_dt += last_dt;
    if (cumulative_dt >= refresh_period)
    {
        cumulative_dt -= refresh_period;
        selected_screen->ProcessQueuedControls();
        display->ClearDisplay();
        selected_screen->Update(last_dt);
        if (enable_cursor)
            cursor->GetComponent()->Update(last_dt, selected_screen);
        display->UpdateDisplay();
    }
}

void ScreenManager::Update(float dt_override)
{
    cumulative_dt += dt_override;
    if (cumulative_dt >= refresh_period)
    {
        cumulative_dt -= refresh_period;
        display->ClearDisplay();
        selected_screen->Update(dt_override);
        display->UpdateDisplay();
    }
}

void ScreenManager::UpdateIfAnyComponentMoving()
{
    // at least one component globally is moving.
    // this function is intended to be called in the main loop
    // therefore it is ideal we guard the set search
    if (master_component_moving_reference_count > 0)
    {
        for (auto s : screen_set)
        {
            if (s->HasMovingComponent())
            {
                if (s == selected_screen)
                {
                    Update(last_dt);
                    continue;
                }
                s->Update(last_dt);
            }
        }        
    }
}

void ScreenManager::UpdateIfCursorActivity()
{
    if (enable_cursor)
    {
        Vec2f dir;

        if (cursor->up_dev->IsActivated())
            dir.y += 1.f;
        if (cursor->down_dev->IsActivated())
            dir.y += -1.f;
        if (cursor->left_dev->IsActivated())
            dir.x += -1.f;
        if (cursor->right_dev->IsActivated())
            dir.x += 1.f;

        if (dir.x == 0.f && dir.y == 0.f)
            return;

        dir = dir.Normalize();

        Component* comp = cursor->GetComponent();

        const Vec2i32 pos = comp->GetOriginPosition();
        comp->SetOriginPosition(pos + static_cast<Vec2i32>(dir * (last_dt * cursor->velocity)));

        for (auto c : selected_screen->components)
        {
            if (c->selectable)
            {
                if (comp->GetDrawDimensions().Intersects(c->GetDrawDimensions()))
                    selected_screen->HoverComponent((SelectableComponent*)c, true);
            }
        }
        
    }
}