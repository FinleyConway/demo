#pragma once

#include "ecs/core/entity.hpp"
#include "ecs/components/health.hpp"
#include "ecs/components/grid_data.hpp"
#include "ecs/components/rendering.hpp"
#include "ecs/components/positional.hpp"
#include "ecs/components/defence_turret.hpp"

namespace demo::ecs {
    struct test_turret : entity<test_turret> {
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

            // turret logic
            registry.emplace<turret>(entity);
            registry.emplace<attack_area>(entity, attack_area {
                .shape = attack_shape::circle,
                .radius = 2.5,
            });
            registry.emplace<turret_search>(entity, turret_search {
                .attack_intervals = 0.5f,
            });
            registry.emplace<turret_attack_damage>(entity, turret_attack_damage {
                .value = 20
            });

            // health
            registry.emplace<health>(entity, health {
                .current_health = 100,
                .max_health = 100
            });
        }
    };
}