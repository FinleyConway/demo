#pragma once

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

namespace demo::ecs {
    struct heal_event {
        entt::entity entity;
        float amount = 0.0f;
    };

    struct heal {
        float amount = 0.0f;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(heal, amount);
    };
}