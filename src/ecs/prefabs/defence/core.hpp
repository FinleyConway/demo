#pragma once

#include "ecs/core/entity_builder.hpp"

#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/rendering/components/sprite_renderer.hpp"
#include "ecs/features/settlement_builder/components/settlement_core.hpp"

namespace demo::ecs {
    struct core : entity_builder {
        entt::entity build(entt::registry& registry, const asset_context& ctx) override {
            auto entity = registry.create();
            
            registry.emplace<position>(entity);

            registry.emplace<sprite_renderer>(entity, sprite_renderer {
                .source = {
                    .w = 32,
                    .h = 32,
                }
            });

            registry.emplace<settlement_core_init>(entity);
            registry.emplace<settlement_core>(entity);

            return entity;
        }
    };
}