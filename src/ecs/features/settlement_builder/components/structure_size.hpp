#pragma once

#include <nlohmann/json.hpp>

#include "world/grid/cell.hpp"

namespace demo::ecs {
    struct structure_size {
        cell value;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(structure_size, value);
    };
}