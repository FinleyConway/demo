#pragma once

#include "ecs/core/prefab_builder.hpp"

#include "ecs/features/health/components/health.hpp"
#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/rendering/components/sprite_renderer.hpp"
#include "ecs/features/settlement_builder/components/structure_size.hpp"

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
            registry.emplace<structure_size>(entity, structure_size {
                .value = cell(1, 1) // occupies 1 cell
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