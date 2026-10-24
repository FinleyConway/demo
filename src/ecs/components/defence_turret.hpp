#pragma once

#include <vector>
#include <cstdint>

#include <entt/entt.hpp>

#include "grid/cell.hpp"

namespace demo::ecs {
    enum class attack_shape {
        circle,
        sector
    };

    struct attack_area {
        attack_shape shape = attack_shape::circle;

        float radius = 0.0f;
        uint16_t angle = 0; // sector direction in degrees
        uint16_t width = 0; // sector width in degrees

        std::vector<cell> cells;
    };

    struct attack_area_turn {
        uint16_t angle = 0;
    };

    struct turret_search {
        entt::entity target = entt::null;
        cell target_cell;

        float attack_intervals = 1.0f;
        float attack_time = 0.0f;
    };

    struct turret_attack {
        entt::entity protential_target = entt::null;
        cell target_cell;
    };

    // maybe different attack types?
    // nearest, strongest, first etc

    // maybe attack behaviour?
    // instant, splash, projectile etc
}