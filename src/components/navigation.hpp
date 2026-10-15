#pragma once

#include <vector>

#include "grid/position/cell.hpp"
#include "grid/position/point.hpp"
#include "grid/pathing/path_finder.hpp"

namespace demo {
    struct auto_navigation {}; // tag

    struct navigation_target {
        cell value;
    };

    struct navigation_path {
        std::vector<cell> cells;
    };

    struct navigation_motion {
        point start_position;
        float elapsed = 0.0f;
        float time_per_cell = 0.15f;
    };

    struct navigation_request_state {
        path_request request;
    };
}