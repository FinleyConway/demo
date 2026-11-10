#pragma once

#include <nlohmann/json.hpp>

namespace demo::ecs {
    struct camera_zoom {
        float increments = 0.35f;
        float min = 1.0f;
        float max = 4.0f;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(camera_zoom, 
            increments,
            min,
            max
        );
    };
}