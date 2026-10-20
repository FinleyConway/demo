#pragma once

#include "math/point.hpp"

namespace demo {
    struct aabb {
        point lower_bounds;
        point upper_bounds;

        constexpr bool valid() const {
            return 
                lower_bounds.x <= upper_bounds.x && 
                lower_bounds.y <= upper_bounds.y;
        }
    };
}