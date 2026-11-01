#pragma once

#include "ecs/core/entity_builder.hpp"
#include "ecs/components/health.hpp"
#include "ecs/components/grid_data.hpp"
#include "ecs/components/rendering.hpp"
#include "ecs/components/positional.hpp"

namespace demo::ecs {
    struct wall : entity_builder {
        void build(entt::registry& registry, const asset_context& ctx) override {
            auto entity = registry.create();
            
            // positional
            registry.emplace<world_position>(entity);

            // visuals
            registry.emplace<sprite>(entity, sprite {
                .source = {
                    .w = 32,
                    .h = 32,
                }
            });

            // grid logic
            registry.emplace<grid_size>(entity, grid_size {
                .value = cell(1, 1) // occupies 1 cell
            });

            // health
            registry.emplace<health>(entity, health {
                .current_health = 100,
                .max_health = 100
            });
        }
    };
}