#pragma once

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

namespace demo::ecs {
    struct damage_event {
        entt::entity entity;
        float amount = 0.0f;
    };

    struct damage {
        float amount = 0.0f;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(damage, amount);
    };
}