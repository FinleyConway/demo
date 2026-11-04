#pragma once

#include <vector>

#include "world/grid/cell.hpp"

namespace demo {
    struct attack_area_init {};

    struct attack_area {
        std::vector<cell> cells;
    };
}