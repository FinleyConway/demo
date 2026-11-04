#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/health/components/health.hpp"
#include "ecs/features/health/components/health_events.hpp"

namespace demo::ecs {
    class apply_damage : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<health, take_damage>();

            for (auto [e, health, damage] : view.each()) {
                health.current -= damage.amount;

                if (health.current <= 0.0f) {
                    health.current = 0.0f;

                    registry().emplace<killed>(e);
                }

                registry().remove<take_damage>(e);
            }
        }
    };
}