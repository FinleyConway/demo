#pragma once

#include "world/grid/cell.hpp"
#include "world/path_finding/movement_profile.hpp"

namespace demo::ecs {
    struct navigation_target {
        cell value;
    };

    struct navigation_profile {
        movement_cost costs;
    };

    struct navigation_wait {};
}