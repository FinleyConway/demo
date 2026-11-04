#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/map/components/map.hpp"
#include "ecs/features/rendering/components/tile_renderer.hpp"

namespace demo::ecs {
    class setup_map : public system_base {
    public:
        using system_base::system_base;

        void start() override {
            auto view = registry().view<
                map_init, 
                map, 
                tile_renderer
            >();

            for (auto [e, tile] : view.each()) {
                create_visuals(e, tile);
            }
        }

    private:
        void create_visuals(entt::entity e, tile_renderer& tile) {
            const auto& grid = context<world_grid>();
            const cell grid_size = { grid.width(), grid.height() };

            grid.for_each_cell({}, grid_size, [&](cell cell) {
                rect_int source = ((cell.x + cell.y) % 2 == 0) 
                    ? rect_int(0 , 0, 32, 32)
                    : rect_int(32, 0, 32, 32);

                //! need to setup tile rendering events
                // tile.add_tile(tile_renderer::tile {
                //     .cell = cell,
                //     .source = source
                // }, registry(), e);
            });
        }
    };
}