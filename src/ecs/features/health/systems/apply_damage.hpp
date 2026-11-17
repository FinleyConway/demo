#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/health/components/damage.hpp"
#include "ecs/features/health/components/health.hpp"
#include "ecs/features/health/components/killed.hpp"

namespace demo::ecs {
    class apply_damage : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<health, take_damage>();

            for (auto [e, health, damage] : view.each()) {
                // apply damage making sure its not negitive
                health.current = std::max(
                    0.0f,
                    health.current - damage.amount
                );

                if (health.current <= 0.0f) {
                    emit<killed>(e);
                }
            }
        }
    };
}