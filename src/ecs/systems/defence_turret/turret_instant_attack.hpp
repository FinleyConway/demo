#pragma once

#include "ecs/systems/system_base.hpp"

#include "ecs/components/health.hpp"
#include "ecs/components/defence_turret.hpp"

namespace demo::ecs {
    class turret_instant_attack : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<turret_attack, turret_attack_damage>();

            for (auto [e, attack, damage] : view.each()) {
                entt::entity target = attack.protential_target;

                if (registry().all_of<health>(target)) {
                    auto& td = registry().get_or_emplace<take_damage>(target, take_damage {
                        .amount = damage.value
                    });

                    td.amount += damage.value;
                }

                registry().remove<turret_attack>(e);
            }
        }
    };
}