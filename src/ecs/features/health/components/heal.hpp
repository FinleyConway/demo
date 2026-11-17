#pragma once

#include <nlohmann/json.hpp>

namespace demo::ecs {
    struct heal_providing {
        float amount = 0.0f;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(heal_providing, amount);
    };

    struct heal {
        float amount = 0.0f;
    };
}