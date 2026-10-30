#pragma once

#include "world/grid/grid.hpp"
#include "world/world_grid/tile.hpp"

namespace demo {
    using world_grid = grid<tile, 30, 30, 32>;
}