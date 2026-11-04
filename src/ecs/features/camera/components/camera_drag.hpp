#pragma once

#include "core/math/point.hpp"

namespace demo::ecs {
    struct camera_drag {
        point last_mouse_position;
    };
}