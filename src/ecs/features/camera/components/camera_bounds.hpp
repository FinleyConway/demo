#pragma once

#include "core/math/point.hpp"

namespace demo::ecs {
    struct camera_bounds_init {};

    struct camera_bounds {
        point min;
        point max;
    };
}