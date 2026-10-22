#pragma once

#include "math/point.hpp"

namespace demo::ecs {
    struct camera_control {
        float zoom_increments = 0.35f;
        float min_zoom = 1.0f;
        float max_zoom = 4.0f;

        point last_mouse_position;
    };
}