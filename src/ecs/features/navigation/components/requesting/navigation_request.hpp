#pragma once

#include <nlohmann/json.hpp>

#include "world/grid/cell.hpp"
#include "world/path_finding/movement_profile.hpp"

namespace demo::ecs {
    struct navigation_target {
        cell start;
        cell end;
    };

    struct navigation_profile {
        movement_cost costs;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(navigation_profile, costs);
    };
}