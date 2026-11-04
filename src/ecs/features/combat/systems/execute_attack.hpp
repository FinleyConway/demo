#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/combat/components/attack.hpp"
#include "ecs/features/combat/components/attack_target.hpp"

namespace demo::ecs {
    class execute_attack : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<
                attack_target,
                attack_cooldown
            >();

            for (auto [e, target, cooldown] : view.each()) {
                cooldown.remaining += dt;

                if (cooldown.remaining < cooldown.duration) continue;

                if (!registry().valid(target.entity)) continue;

                registry().emplace_or_replace<attack>(
                    e,
                    attack {
                        .target = target.entity,
                        .target_cell = target.target_cell
                    }
                );

                cooldown.remaining = 0.0f;
            }
        }
    };
}