#pragma once

#include "ecs/core/entity_builder.hpp"

#include "ecs/core/prefab_builder.hpp"
#include "ecs/features/combat/combat_setup.hpp"
#include "ecs/features/health/components/health.hpp"
#include "ecs/features/combat/components/attack_type.hpp"
#include "ecs/features/combat/components/attack_target.hpp"
#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/combat/components/attack_pattern.hpp"
#include "ecs/features/rendering/components/tile_renderer.hpp"
#include "ecs/features/rendering/components/sprite_renderer.hpp"
#include "ecs/features/settlement_builder/settlement_builder_setup.hpp"

namespace demo::ecs {
    struct test_turret : prefab_builder {
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
            settlement_feature::make_structure(registry, entity, {
                .is_core = false,
                .size = cell(1, 1)
            });

            // turret logic
            combat_feature::make_combat(registry, entity, {
                .attack_shape = attack_pattern::shape::circle,
                .radius =  2.5f,
                .angle = 0,
                .width = 0,
                .attack_cooldown = 1.0f,
                .attack_pref = attack_preference::type::enemies,
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