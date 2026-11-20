#pragma once

#include <vector>

#include "world/grid/cell.hpp"

namespace demo::ecs {
    struct navigation_path {
        std::vector<cell> cells;
    };
}