#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/health/components/health.hpp"
#include "ecs/features/health/components/health_events.hpp"

namespace demo::ecs {
    class apply_healing : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<health, heal>();

            for (auto [e, health, heal] : view.each()) {
                health.current += heal.amount;

                if (health.current > health.max) {
                    health.current = health.max;
                }
            }
        }
    };
}