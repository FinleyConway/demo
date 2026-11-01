#pragma once

#include "ecs/core/entity_builder.hpp"
#include "ecs/components/rendering.hpp"
#include "ecs/components/positional.hpp"
#include "ecs/components/defence_pillar.hpp"

namespace demo::ecs {
    struct pillar : entity_builder {
        entt::entity build(entt::registry& registry, const asset_context& ctx) override {
            auto entity = registry.create();
            
            registry.emplace<world_position>(entity);

            registry.emplace<sprite>(entity, sprite {
                .source = {
                    .w = 32,
                    .h = 32,
                }
            });

            registry.emplace<defence_pillar>(entity);

            return entity;
        }
    };
}