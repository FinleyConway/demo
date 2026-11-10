#pragma once

#include <nlohmann/json.hpp>

namespace demo::ecs {
    struct health {
        float current = 100.0f;
        float max = 100.0f;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(health, current, max);
    };

    struct damage {
        float amount = 0.0f;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(damage, amount);
    };
}