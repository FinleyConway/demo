#pragma once

#include "ecs/core/prefab_builder.hpp"

#include "ecs/components/enemy.hpp"
#include "ecs/components/positional.hpp"
#include "ecs/components/animation.hpp"
#include "ecs/components/rendering.hpp"
#include "ecs/components/navigation.hpp"
#include "ecs/components/health.hpp"

#include "resources/asset_context.hpp"

namespace demo::ecs {
    struct zombie : prefab_builder {
        entt::entity build(entt::registry& registry, const asset_context& ctx) override {
            auto entity = registry.create();
            
            registry.emplace<world_position>(entity);
            registry.emplace<sprite>(entity, sprite {
                &ctx.test.source, { 0, 0, 40, 40 }
            });
            registry.emplace<animator>(entity, animator {
                .clip = &ctx.test.walking
            });

            registry.emplace<navigation_profile>(entity, navigation_profile {
                .costs = {
                    .wall = 15
                }
            });
            registry.emplace<navigation_path>(entity);
            registry.emplace<navigation_motion>(entity);
            registry.emplace<navigation_request_state>(entity);

            registry.emplace<health>(entity);
            
            registry.emplace<enemy_attack>(entity);
            registry.emplace<enemy_attack_damage>(entity, enemy_attack_damage {
                .value = 20
            });

            return entity;
        }
    }; 
}