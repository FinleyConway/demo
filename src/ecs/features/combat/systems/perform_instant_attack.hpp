#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/health/components/damage.hpp"
#include "ecs/features/health/components/health.hpp"
#include "ecs/features/combat/components/attack.hpp"
#include "ecs/features/combat/components/attack_type.hpp"

namespace demo::ecs {
    class perform_instant_attack : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<
                instant_attack, 
                attack,
                damage_dealing
            >();

            for (auto [e, atk, damage] : view.each()) {
                if (!registry().all_of<health>(atk.target)) continue;

                emit<take_damage>(e, take_damage {
                    .amount = damage.amount
                });
            }

            registry().clear<attack>();
        }
    };
}