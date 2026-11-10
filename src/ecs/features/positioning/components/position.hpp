#pragma once

#include "core/math/point.hpp"

#include <nlohmann/json.hpp>

namespace demo::ecs {
    struct position {
        point value;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(position,
            value
        )
    };

    struct set_position {
        point value;
    };
}