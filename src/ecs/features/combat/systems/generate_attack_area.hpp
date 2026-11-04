#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/combat/components/area_attack.hpp"
#include "ecs/features/combat/components/attack_pattern.hpp"
#include "ecs/features/rendering/components/tile_renderer.hpp"

#include "world/grid/cell.hpp"
#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    class generate_attack_area : public system_base {
    public:
        using system_base::system_base;

        void start() override {
            auto view = registry().view<
                attack_area_init,
                attack_pattern,
                attack_area
            >();

            for (auto [e, pattern, area] : view.each()) {
                const std::vector<spatial_cell> cells = get_cells(pattern);

                // provide attack visuals if entity has tile renderer
                try_add_visuals(e, cells);

                area.cells = std::move(cells);

                registry().remove<attack_area_init>(e);
            }
        }

    private:
        void try_add_visuals(entt::entity e, const  std::vector<spatial_cell>& cells) {
            if (!registry().all_of<tile_renderer>(e)) return;
            
            std::vector<tile_renderer::tile> tiles;
            tiles.reserve(cells.size());

            for (const auto& sc : cells) {
                // add tile for each cell
                tiles.emplace_back(tile_renderer::tile {
                    .cell_position = sc.to_cell(),
                    .source = { 64, 0, 32, 32 }
                });
            }

            registry().emplace_or_replace<add_renderer_tiles>(e, std::move(tiles));
        }

        std::vector<spatial_cell> get_cells(const attack_pattern& area) const {
            const world_grid& grid = context<world_grid>();

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