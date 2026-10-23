#pragma once

#include <entt/entt.hpp>

#include "grid/grid.hpp"

namespace demo {
    struct tile {
        entt::entity entity = entt::null;

        bool empty() const {
            return entity == entt::null;
        }
    };
    
    using world_grid = grid<tile, 25, 25, 32>;
}