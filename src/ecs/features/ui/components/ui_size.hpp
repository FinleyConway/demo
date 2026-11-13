#pragma once

#include <nlohmann/json.hpp>

#include "core/math/point.hpp"

namespace demo::ecs {
    struct ui_size {
        point value;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(ui_size, value);
    };
}