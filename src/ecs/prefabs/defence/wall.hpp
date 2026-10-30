#pragma once

#include "ecs/core/entity.hpp"
#include "ecs/components/health.hpp"
#include "ecs/components/grid_size.hpp"
#include "ecs/components/rendering.hpp"
#include "ecs/components/positional.hpp"

namespace demo::ecs {
    struct wall : entity<wall> {
        static void add_components(entt::registry& registry, entt::entity entity) {
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