#pragma once

#include "ecs/systems/system_base.hpp"

#include "ecs/components/health.hpp"

namespace demo::ecs {
    class health_addition : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<health, heal>();

            for (auto [e, health, heal] : view.each()) {
                health.current_health += heal.amount;

                if (health.current_health > health.max_health) {
                    health.current_health = health.max_health;
                }
            }
        }
    };
}