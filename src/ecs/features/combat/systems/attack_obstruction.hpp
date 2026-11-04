#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/combat/components/attack_target.hpp"
#include "ecs/features/combat/components/attack_capabilities.hpp"
#include "ecs/features/navigation/components/navigation_obstruction.hpp"

#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    class attack_obstruction : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<
                can_attack_obstructions,
                navigation_obstruction
            >();

            for (auto [e, obstruction] : view.each()) {
                const auto target = context<world_grid>()
                    .get(obstruction.tile)
                    .get_structure();

                if (target == entt::null || !registry().valid(target)) {
                    registry().remove<attack_target>(e);

                    continue;
                }

                if (registry().all_of<attack_target>(e)) continue;

                registry().emplace_or_replace<attack_target>(
                    e,
                    attack_target { 
                        .entity = target,
                        .target_cell = obstruction.tile
                    }
                );
            }
        }
    };
}