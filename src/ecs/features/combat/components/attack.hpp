#pragma once

#include <entt/entt.hpp>

#include "world/grid/cell.hpp"

namespace demo::ecs {
    struct attack {
        entt::entity target = entt::null;
        cell target_cell;
    };
}