#pragma once

#include <cstdint>

#include "ecs/core/prefab_manager.hpp"
#include "world/grid/cell.hpp"

namespace demo::ecs {
    struct horde_target {
        cell target;
    };

    struct horde_info {
        prefab_id horde_enemy;
        uint32_t enemy_count = 0;

        cell spawn_tile;
        float spawn_radius = 0.0f;
        float spawn_chance = 0.0f;
    };

    struct horde_count {
        uint32_t alive = 0;
    };
}