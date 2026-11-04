#pragma once

#include <entt/entt.hpp>

#include "world/grid/cell.hpp"

namespace demo::ecs {
    struct attack_preference {
        enum class type {
            enemies,
            structures,
        };

        type value = type::enemies;
    };

    struct attack_target {
        entt::entity entity = entt::null;
        cell target_cell;
    };

    struct attack_cooldown {
        float duration = 1.0f;
        float remaining = 0.0f;
    };

    // maybe different attack types?
    // nearest, strongest, first etc

    // maybe attack behaviour?
    // instant, splash, projectile etc
}