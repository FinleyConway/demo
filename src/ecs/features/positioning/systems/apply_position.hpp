#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/positioning/components/static_entity.hpp"
#include "ecs/features/positioning/components/dirty_position.hpp"

#include "core/debugging.hpp"

namespace demo::ecs {
    class apply_position : public system_base {
    public:
        using system_base::system_base;

        void on_hook() override {
            registry().on_construct<set_position>().connect<&apply_position::apply>(*this);
        }

    private:    
        void apply(entt::registry& registry, entt::entity e) {
            ASSERT((registry.all_of<position>(e)), 
                "Entity doesn't have a position component!");

            // cant move a static entity
            if (registry.all_of<static_entity>(e)) return;

            const auto& request = registry.get<set_position>(e);
            auto& current = registry.get<position>(e);

            // check if the position actually has changed
            if (current.value != request.value) {
                current.value = request.value;

                // set entity as dirty
                registry.emplace_or_replace<dirty_position>(e);
            }

            //registry.remove<set_position>(e);
        }
    };
}