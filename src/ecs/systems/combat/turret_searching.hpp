#pragma once

#include "ecs/core/system_base.hpp"
#include "ecs/components/positional.hpp"
#include "ecs/components/defence_pillar.hpp"
#include "ecs/components/defence_turret.hpp"

#include "core/debugging.hpp"
#include "world/grid/world_grid.hpp"

namespace demo::ecs {
    class turret_searching : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<
                world_position,
                attack_area,
                turret_search
            >();

            for (auto [e, pos, area, search] : view.each()) {
                if (!registry().valid(search.target)) {
                    auto [found, cell] = search_area(pos, area);

                    if (!registry().valid(found)) continue;

                    search.target = found;
                    search.target_cell = cell;

                    LOG_INFO("Target found: %d", found);
                }
                
                attack_entity(e, search, dt);
            }
        }

    private:
        std::pair<entt::entity, cell> search_area(const world_position& pos, const attack_area& area) {
            const world_grid& grid = context<world_grid>();
            const cell grid_world_pos = grid.point_to_cell(pos.get());

            // simple first come first serve atm
            for (const auto& c : area.cells) {
                const cell cell_offset = {
                    .x = c.x + grid_world_pos.x,
                    .y = c.y + grid_world_pos.y
                };

                if (!grid.in_bounds(cell_offset)) continue;

                const auto& tile = grid.get(cell_offset);
                const auto& entities = tile.entities_in_cell();

                // ignore structure you have to protect and other turrets
                if (entities.empty()) continue;

                return { entities[0], cell_offset };
            }

            return { entt::null, {} };
        }   

        void attack_entity(entt::entity e, turret_search& search, float dt) {
            if (!registry().valid(search.target)) return;

            search.attack_time += dt;

            if (search.attack_time >= search.attack_intervals) {
                // perform attack
                registry().emplace_or_replace<turret_attack>(e, turret_attack {
                    .protential_target = search.target,
                    .target_cell = search.target_cell
                });

                LOG_INFO("Attacked");

                search.attack_time = 0.0f;
                search.target = entt::null;
                search.target_cell = {};
            }
        }
    };
}