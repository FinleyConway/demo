#pragma once

#include "world/grid/cell.hpp"

namespace demo::ecs {
    struct horde_info {
        uint32_t enemy_count = 0;

        cell spawn_tile;
        float spawn_radius = 0.0f;
    };

    struct horde_count {
        uint32_t alive = 0;
    };
}