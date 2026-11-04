#pragma once

#include "ecs/core/entity_builder.hpp"
#include "ecs/components/health.hpp"
#include "ecs/components/grid_data.hpp"
#include "ecs/components/rendering.hpp"
#include "ecs/components/positional.hpp"
#include "ecs/components/defence_turret.hpp"

namespace demo::ecs {
    struct test_turret : entity_builder {
        entt::entity build(entt::registry& registry, const asset_context& ctx) override {
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
            registry.emplace<tile_renderer>(entity, tile_renderer {
                .tex = nullptr
            });

            // grid logic
            registry.emplace<grid_size>(entity, grid_size {
                .value = cell(1, 1) // occupies 1 cell
            });

            // turret logic
            registry.emplace<turret>(entity);
            registry.emplace<turret_attack_area>(entity, turret_attack_area {
                .shape = turret_attack_area::shape::circle,
                .radius = 2.5,
            });
            registry.emplace<turret_search>(entity, turret_search {
                .attack_intervals = 0.5f,
            });

            // health
            registry.emplace<damage>(entity, damage {
                .amount = 20.0f
            });
            registry.emplace<health>(entity, health {
                .current_health = 100.0f,
                .max_health = 100.0f
            });

            return entity;
        }
    };
}