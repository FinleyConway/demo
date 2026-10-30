#pragma once

#include <vector>

#include "core/math/point.hpp"
#include "world/grid/cell.hpp"
#include "world/path_finding/path_finder.hpp"
#include "world/path_finding/movement_profile.hpp"

namespace demo::ecs {
    struct navigation_target {
        cell value;
    };

    struct navigation_profile {
        movement_cost costs;
    };

    struct navigation_path {
        std::vector<cell> cells;
    };

    struct navigation_wait { };

    struct navigation_motion {
        point start_position;
        float elapsed = 0.0f;
        float time_per_cell = 0.15f;
    };

    struct navigation_request_state {
        path_request request;
    };
}