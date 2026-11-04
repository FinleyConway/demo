#pragma once

#include "ecs/core/prefab_builder.hpp"

#include "ecs/features/health/components/health.hpp"
#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/rendering/components/sprite_renderer.hpp"
#include "ecs/features/settlement_builder/settlement_builder_setup.hpp"

namespace demo::ecs {
    struct wall : prefab_builder {
        entt::entity build(entt::registry& registry, const asset_context& ctx) override {
            auto entity = registry.create();
            
            // positional
            registry.emplace<position>(entity);

            // visuals
            registry.emplace<sprite_renderer>(entity, sprite_renderer {
                .source = {
                    .w = 32,
                    .h = 32,
                }
            });

            // grid logic
            settlement_feature::make_structure(registry, entity, {
                .is_core = false,
                .size = { 1, 1 }
            });

            // health
            registry.emplace<health>(entity, health {
                .current = 100.0f,
                .max = 100.0f
            });

            return entity;
        }
    };
}