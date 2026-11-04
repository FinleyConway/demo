#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/combat/components/area_attack.hpp"
#include "ecs/features/combat/components/attack_pattern.hpp"

#include "ecs/components/rendering.hpp"

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
                if (auto* tile = registry().try_get<tile_renderer>(e)) {
                    for (const auto& sc : cells) {
                        // add tile for each cell
                        tile->add_tile({
                            .position = sc.to_cell(),
                            .source = { 64, 0, 32, 32 }
                        }, registry(), e);
                    }
                }

                area.cells = std::move(cells);

                registry().remove<attack_area_init>(e);
            }
        }

    private:
        std::vector<spatial_cell> get_cells(const attack_pattern& area) const {
            const world_grid& grid = context<world_grid>();

            // generate cells basedon attack pattern shape
            switch (area.shape) {
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