#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/health/components/heal.hpp"
#include "ecs/features/health/components/health.hpp"

namespace demo::ecs {
    class apply_healing : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<health, heal>();

            for (auto [e, health, heal] : view.each()) {
                // apply health clamped within the entities max health
                health.current = std::clamp(
                    health.current + heal.amount,
                    0.0f,
                    health.max
                );
            }
        }
    };
}