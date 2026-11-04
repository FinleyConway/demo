#pragma once

#include "world/grid/grid.hpp"
#include "rendering/spatial_id.hpp"
#include "world/world_grid/tile.hpp"
#include "world/grid/spatial_grid.hpp"

namespace demo {
    using world_grid = grid<tile, 30, 30, 32>;
    using spatial_render_grid = spatial_grid<spatial_id, 128>;
}