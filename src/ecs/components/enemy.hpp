#pragma once

#include "world/grid/cell.hpp"

namespace demo::ecs {
    struct route_obstruction {
        cell tile;
    };

    struct enemy_attack {
        float attack_intervals = 1.0f;
        float attack_time = 0.0f;
    };

    struct enemy_attack_damage {
        float value = 0;
    };
}