#pragma once

#include "world/grid/cell.hpp"

namespace demo {
    struct grid_size {
        cell value;
    };

    struct grid_navigation_weight {
        size_t value = 0;
    };
}