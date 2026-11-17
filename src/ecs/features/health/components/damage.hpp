#pragma once

#include <nlohmann/json.hpp>

namespace demo::ecs {
    struct damage_dealing {
        float amount = 0.0f;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(damage_dealing, amount);
    };

    struct take_damage {
        float amount = 0.0f;
    };
}