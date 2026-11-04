#pragma once

#include <vector>

#include "world/grid/cell.hpp"
#include "world/path_finding/path_finder.hpp"

namespace demo::ecs {
    struct navigation_path {
        std::vector<cell> cells;
    };

    struct navigation_request_state {
        path_request request;
    };
}