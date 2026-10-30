#pragma once

#include "ecs/core/entity.hpp"
#include "ecs/components/positional.hpp"
#include "ecs/components/animation.hpp"
#include "ecs/components/rendering.hpp"
#include "ecs/components/navigation.hpp"
#include "ecs/components/health.hpp"

#include "resources/asset_context.hpp"

namespace demo::ecs {
    struct zombie : entity<zombie> {
        static void add_components(entt::registry& registry, entt::entity entity, const asset_context& ctx) {
            registry.emplace<world_position>(entity);
            registry.emplace<sprite>(entity, sprite {
                &ctx.test.source, { 0, 0, 40, 40 }
            });
            registry.emplace<animator>(entity, animator {
                .clip = &ctx.test.walking
            });

            registry.emplace<navigation_profile>(entity);
            registry.emplace<navigation_path>(entity);
            registry.emplace<navigation_motion>(entity);
            registry.emplace<navigation_request_state>(entity);

            registry.emplace<health>(entity);
            
        }
    }; 
}