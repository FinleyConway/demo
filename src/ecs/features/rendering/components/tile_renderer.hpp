#pragma once 

#include <vector>

#include "core/math/rect.hpp"
#include "core/math/point.hpp"
#include "world/grid/aabb.hpp"
#include "world/grid/cell.hpp"
#include "rendering/colour.hpp"
#include "resources/asset_context.hpp"
#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    struct tile_renderer_init {};

    struct tile_renderer_changed {};

    struct tile_renderer {
        struct tile {
            cell cell_position;
            rect_int source;
            colour tint = colour::white;
        };

        asset_id texture = invalid_asset_id;
        std::vector<tile> tiles;
        bool visible = true;

        constexpr aabb make_bounds(const point& position, world_grid& grid) const {
            if (tiles.empty()) return { position, {} };

            point min = position + grid.cell_to_point(tiles.front().cell_position);
            point max = min;

            for (const auto& tile : tiles) {
                const point tile_world = position + grid.cell_to_point(tile.cell_position);

                min.x = std::min(min.x, tile_world.x);
                min.y = std::min(min.y, tile_world.y);

                max.x = std::max(max.x, tile_world.x);
                max.y = std::max(max.y, tile_world.y);
            }

            return { min, max };
        }
    };

    struct add_renderer_tiles {
        std::vector<tile_renderer::tile> values;
    };
}