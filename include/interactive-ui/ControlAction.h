#pragma once

enum ControlAction : uint64_t
{
    SELECT0 = 1 << 0,
    SELECT1 = 1 << 1,
    BACK = 1 << 2,
    DIRECTIONAL_UP = 1 << 3,
    DIRECTIONAL_DOWN = 1 << 4,
    DIRECTIONAL_LEFT = 1 << 5,
    DIRECTIONAL_RIGHT = 1 << 6
};