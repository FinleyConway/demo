#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/components/enemy.hpp"
#include "ecs/components/health.hpp"
#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    class obstruction_clearance : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<
                enemy_attack,
                enemy_attack_damage,
                route_obstruction
            >();

            for (auto [e, attack, damage, obstr] : view.each()) {
                attack.attack_time += dt;

                if (attack.attack_time >= attack.attack_intervals) {
                    const world_grid& grid = context<world_grid>();
                    entt::entity structure = grid.get(obstr.tile).get_structure();

                    registry().emplace_or_replace<take_damage>(structure, take_damage {
                        .amount = damage.value
                    });

                    attack.attack_time = 0.0f;

                    registry().remove<route_obstruction>(e);
                }
            }
        }
    };
}