#pragma once

#include "core/math/point.hpp"

namespace demo::ecs {
    struct position {
        point value;
    };

    struct set_position {
        point value;
    };
}