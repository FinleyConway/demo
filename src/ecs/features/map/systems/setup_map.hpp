#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/map/components/map.hpp"
#include "ecs/features/rendering/components/tile_renderer.hpp"

namespace demo::ecs {
    class setup_map : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<
                map_init, 
                map
            >();

            for (auto e : view) {
                create_visuals(e);

                registry().remove<map_init>(e);
            }
        }

    private:
        void create_visuals(entt::entity e) {
            const auto& grid = context<world_grid>();
            const cell grid_size = { grid.width(), grid.height() };

            std::vector<tile_renderer::tile> tiles;
            tiles.reserve(grid.width() * grid.height());

            grid.for_each_cell({}, grid_size, [&](cell cell) {
                const rect_int source = ((cell.x + cell.y) % 2 == 0) 
                    ? rect_int(0 , 0, 32, 32)
                    : rect_int(32, 0, 32, 32);
                
                tiles.emplace_back(tile_renderer::tile {
                    .cell_position = cell,
                    .source = source
                });
            });

            registry().emplace_or_replace<add_renderer_tiles>(e, add_renderer_tiles {
                .values = std::move(tiles)
            });
        }
    };
}