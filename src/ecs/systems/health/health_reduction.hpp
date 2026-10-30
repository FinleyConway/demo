#pragma once

#include "ecs/core/system_base.hpp"
#include "ecs/components/health.hpp"

#include "core/debugging.hpp"

namespace demo::ecs {
    class health_reduction : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<health, take_damage>();

            for (auto [e, health, damage] : view.each()) {
                health.current_health -= damage.amount;

                LOG_INFO("Current health: %f", health.current_health);

                if (health.current_health <= 0.0f) {
                    health.current_health = 0.0f;

                    registry().emplace<killed>(e);
                }

                registry().remove<take_damage>(e);
            }
        }
    };
}