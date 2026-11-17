#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/combat/components/attack_area.hpp"
#include "ecs/features/combat/components/attack_pattern.hpp"
#include "ecs/features/entity_init/components/entity_spawned.hpp"
#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/rendering/components/tile_renderer.hpp"

#include "world/grid/cell.hpp"
#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    class generate_attack_area : public system_base {
    public:
        using system_base::system_base;

        static constexpr phase phase_v = phase::start;

    public:
        void update(float dt) override {
            auto view = registry().view<
                entity_spawned,
                position,
                attack_pattern,
                attack_area
            >();

            for (auto [e, pos, pattern, area] : view.each()) {
                const std::vector<cell> cells = get_cells(pattern);

                // provide attack visuals if entity has tile renderer
                try_add_visuals(e, pos, cells);

                area.cells = std::move(cells);
            }
        }

    private:
        void try_add_visuals(entt::entity e, const position& pos, const std::vector<cell>& cells) {
            if (!registry().all_of<tile_renderer>(e)) return;
            
            registry().patch<tile_renderer>(e, [&](auto& renderer) {
                renderer.tiles.reserve(cells.size());
                
                for (const auto& cell : cells) {
                    // add tile for each cell
                    renderer.tiles.emplace_back(tile_renderer::tile {
                        .cell_position = cell,
                        .source = { 64, 0, 32, 32 },
                        .tint = { 255, 255, 255, 128 }
                    });
                }
            });
        }

        std::vector<cell> get_cells(const attack_pattern& area) const {
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