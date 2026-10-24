#pragma once

#include "ecs/systems/system_base.hpp"

#include "ecs/components/health.hpp"

namespace demo::ecs {
    class entity_killed : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<killed>();

            for (auto e : view) {
                registry().destroy(e);
            }
        }
    };
}