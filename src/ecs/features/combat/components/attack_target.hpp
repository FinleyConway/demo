#pragma once

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "world/grid/cell.hpp"

namespace demo::ecs {
    struct attack_preference {
        enum class type {
            enemies,
            structures,
        };

        type value = type::enemies;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(attack_preference,
            value
        );
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(attack_preference::type, {
        {attack_preference::type::enemies, "enemies"},
        {attack_preference::type::structures, "structures"},
    });

    struct attack_cooldown {
        float duration = 1.0f;
        float remaining = 0.0f;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(attack_cooldown,
            duration
        );
    };

    struct attack_target {
        entt::entity entity = entt::null;
        cell target_cell;
    };

    // maybe different attack types?
    // nearest, strongest, first etc

    // maybe attack behaviour?
    // instant, splash, projectile etc
}