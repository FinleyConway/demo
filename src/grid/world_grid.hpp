#pragma once

#include "grid/grid.hpp"

namespace demo {
    struct tile {
        bool walkable = true;
    };
    
    using world_grid = grid<tile, 100, 100, 32>;
}