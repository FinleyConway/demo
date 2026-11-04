#pragma once

#include "ecs/core/entity_builder.hpp"

#include "ecs/features/health/components/health.hpp"
#include "ecs/features/combat/components/area_attack.hpp"
#include "ecs/features/combat/components/attack_type.hpp"
#include "ecs/features/combat/components/attack_target.hpp"
#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/combat/components/attack_pattern.hpp"
#include "ecs/features/rendering/components/tile_renderer.hpp"
#include "ecs/features/rendering/components/sprite_renderer.hpp"
#include "ecs/features/settlement_builder/components/structure_size.hpp"

namespace demo::ecs {
    struct test_turret : entity_builder {
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
            registry.emplace<tile_renderer>(entity, tile_renderer {
                .tex = nullptr
            });

            // building
            registry.emplace<structure_size>(entity, structure_size {
                .value = cell(1, 1) // occupies 1 cell
            });

            // turret logic
            //registry.emplace<can_attack_entities>(entity);

            registry.emplace<attack_area_init>(entity);
            registry.emplace<attack_area>(entity);
            registry.emplace<attack_pattern>(entity, attack_pattern {
                .shape = attack_pattern::shape::circle,
                .radius = 2.5f,
            });
            registry.emplace<attack_preference>(entity, attack_preference {
                .value = attack_preference::type::enemies
            });
            registry.emplace<attack_cooldown>(entity, attack_cooldown {
                .duration = 1.0f
            });

            registry.emplace<instant_attack>(entity);

            // health
            registry.emplace<damage>(entity, damage {
                .amount = 20.0f
            });
            registry.emplace<health>(entity, health {
                .current = 100.0f,
                .max = 100.0f
            });

            return entity;
        }
    };
}