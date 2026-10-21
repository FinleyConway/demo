#pragma once

#include "ecs/entity/entity.hpp"

#include "ecs/components/positional.hpp"
#include "ecs/components/animation.hpp"
#include "ecs/components/rendering.hpp"
#include "ecs/components/navigation.hpp"

#include "resource/test_assets.hpp"

namespace demo::ecs {
    struct zombie : entity<zombie> {
        static void add_components(entt::registry& registry, entt::entity entity, const test_assets& asset) {
            registry.emplace<world_position>(entity);
            registry.emplace<sprite>(entity, sprite {
                &asset.source, { 0, 0, 40, 40 }
            });
            registry.emplace<animator>(entity, animator {
                .clip = &asset.walking
            });

            registry.emplace<auto_navigation>(entity);
            registry.emplace<navigation_target>(entity);
            registry.emplace<navigation_path>(entity);
            registry.emplace<navigation_motion>(entity);
            registry.emplace<navigation_request_state>(entity);
        }
    }; 
}