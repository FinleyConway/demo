#pragma once

#include "core/debugging.hpp"
#include "ecs/core/system_base.hpp"

#include "ecs/features/combat/components/area_attack.hpp"
#include "ecs/features/combat/components/attack_pattern.hpp"
#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/rendering/components/tile_renderer.hpp"

#include "world/grid/cell.hpp"
#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    class generate_attack_area : public system_base {
    public:
        using system_base::system_base;

        void start() override {
            auto view = registry().view<
                position,
                attack_area_init,
                attack_pattern,
                attack_area
            >();

            for (auto [e, pos, pattern, area] : view.each()) {
                const std::vector<spatial_cell> cells = get_cells(pattern);

                // provide attack visuals if entity has tile renderer
                try_add_visuals(e, pos, cells);

                area.cells = std::move(cells);

                registry().remove<attack_area_init>(e);
            }
        }

    private:
        void try_add_visuals(entt::entity e, const position& pos, const std::vector<spatial_cell>& cells) {
            if (!registry().all_of<tile_renderer>(e)) return;
            
            const auto& grid = context<world_grid>();
            const cell grid_pos = grid.point_to_cell(pos.value);

            std::vector<tile_renderer::tile> tiles;
            tiles.reserve(cells.size());

            for (const auto& sc : cells) {
                //! need use int64_t as the cell and not size_t
                const int64_t x = sc.x + grid_pos.x; // shouldnt have to offset with pos?
                const int64_t y = sc.y + grid_pos.y;

                LOG_INFO("[%d, %d]", x, y);

                if (x < 0 || y < 0) continue;

                // add tile for each cell
                tiles.emplace_back(tile_renderer::tile {
                    .cell_position = {
                        .x = (size_t)x,
                        .y = (size_t)y
                    },
                    .source = { 64, 0, 32, 32 }
                });
            }

            registry().emplace_or_replace<add_renderer_tiles>(e, add_renderer_tiles{
                .values = std::move(tiles)
            });
        }

        std::vector<spatial_cell> get_cells(const attack_pattern& area) const {
            const auto& grid = context<world_grid>();

            // generate cells basedon attack pattern shape
            switch (area.attack_shape) {
                case attack_pattern::shape::circle:
                    return grid.cells_in_radius(area.radius);

                case attack_pattern::shape::sector:
                    return grid.cells_in_sector(area.angle, area.width, area.radius);

                default:
                    return {};
            }
        }
    };
}