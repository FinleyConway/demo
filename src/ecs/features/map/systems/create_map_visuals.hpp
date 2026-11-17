#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/map/components/map.hpp"
#include "ecs/features/rendering/components/tile_renderer.hpp"

namespace demo::ecs {
    class create_map_visuals : public system_base {
    public:
        using system_base::system_base;

        static constexpr phase phase_v = phase::start;

    public:
        void update(float dt) override {
            auto view = registry().view<
                entity_spawned,
                map,
                tile_renderer
            >();

            for (auto [e, tiles] : view.each()) {
                create_visuals(e);
            }
        }

    private:
        void create_visuals(entt::entity e) {
            const auto& grid = context<world_grid>();
            const cell grid_size = { grid.width(), grid.height() };

            registry().patch<tile_renderer>(e, [&](auto& renderer) {
                renderer.tiles.reserve(grid.width() * grid.height());

                grid.for_each_cell({}, grid_size, [&](cell cell) {
                    const rect_int source = ((cell.x + cell.y) % 2 == 0) 
                        ? rect_int(0 , 0, 32, 32)
                        : rect_int(32, 0, 32, 32);
                    
                    renderer.tiles.emplace_back(tile_renderer::tile {
                        .cell_position = cell,
                        .source = source
                    });
                });
            });
        }
    };
}