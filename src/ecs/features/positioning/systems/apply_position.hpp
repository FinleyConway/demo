#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/positioning/components/is_static.hpp"
#include "ecs/features/positioning/components/dirty_position.hpp"

namespace demo::ecs {
    class apply_position : public system_base {
    public:
        using system_base::system_base;

        void on_hook() override {
            registry().on_update<position>().connect<&apply_position::apply>(*this);
        }

    private:    
        void apply(entt::registry& registry, entt::entity e) {
            // cant move a static entity
            if (!registry.all_of<is_static>(e)) {
                // set entity as dirty
                registry.emplace_or_replace<dirty_position>(e);
            }
        }
    };
}