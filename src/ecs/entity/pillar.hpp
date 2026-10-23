#pragma once

#include "ecs/entity/entity.hpp"

#include "ecs/components/rendering.hpp"
#include "ecs/components/positional.hpp"
#include "ecs/components/defence_pillar.hpp"

namespace demo::ecs {
    struct pillar : entity<pillar> {
        static void add_components(entt::registry& registry, entt::entity entity) {
            registry.emplace<world_position>(entity);

            registry.emplace<sprite>(entity, sprite {
                .source = {
                    .w = 32,
                    .h = 32,
                }
            });

            registry.emplace<defence_pillar>(entity);
        }
    };
}