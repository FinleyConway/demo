#pragma once

#include "ecs/systems/system_base.hpp"

#include "ecs/components/defence_turret.hpp"

#include "entt/entity/entity.hpp"
#include "grid/world_grid.hpp"

namespace demo::ecs {
    class turret_searching : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<
                attack_area,
                turret_search
            >();

            for (auto [e, area, search] : view.each()) {
                if (!registry().valid(search.target)) {
                    auto [e, cell] = search_area(area);

                    search.target = e;
                    search.target_cell = cell;
                }
                
                attack_entity(e, search, dt);
            }
        }

    private:
        std::pair<entt::entity, cell> search_area(const attack_area& area) {
            const world_grid& grid = context<world_grid>();

            // simple first come first serve atm
            for (const auto& cell : area.cells) {
                if (!grid.in_bounds(cell)) continue;

                const auto& tile = grid.get(cell);

                if (!tile.empty()) continue;

                return { tile.get_entity(), cell };
            }

            return { entt::null, {} };
        }   

        void attack_entity(entt::entity e,turret_search& search, float dt) {
            if (!registry().valid(search.target)) return;

            search.attack_time += dt;

            if (search.attack_time >= search.attack_intervals) {
                // perform attack
                registry().emplace<turret_attack>(e, turret_attack {
                    .protential_target = search.target,
                    .target_cell = search.target_cell
                });

                search.attack_time = 0.0f;
            }
        }
    };
}