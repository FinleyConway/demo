#pragma once

#include <vector>

#include "core/math/point.hpp"
#include "world/grid/cell.hpp"
#include "world/path_finding/path_finder.hpp"
#include "world/path_finding/movement_profile.hpp"

namespace demo::ecs {
    struct auto_navigation {}; // tag

    struct navigation_target {
        cell value;

        static bool set_target(entt::registry& registry, entt::entity entity, const cell& target) {
            if (!registry.all_of<auto_navigation>(entity)) return false;

            registry.patch<navigation_target>(
                entity,
                [&target](navigation_target& component) {
                    component.value = target;
                }
            );

            return true;
        }
    };

    struct navigation_profile {
        movement_cost costs;
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