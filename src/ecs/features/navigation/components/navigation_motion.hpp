#pragma once

#include <nlohmann/json.hpp>

#include "core/math/point.hpp"

namespace demo::ecs {
    struct navigation_motion {
        point start_position;
        float elapsed = 0.0f;
        float time_per_cell = 0.15f;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(navigation_motion, time_per_cell);
    };
}