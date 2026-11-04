#pragma once

#include "ecs/core/entity_builder.hpp"

#include "ecs/features/camera/components/camera_bounds.hpp"
#include "ecs/features/camera/components/camera_drag.hpp"
#include "ecs/features/camera/components/camera_zoom.hpp"

namespace demo::ecs {
    struct main_camera : entity_builder {
        entt::entity build(entt::registry& registry, const asset_context& ctx) override {
            auto entity = registry.create();
            
            registry.emplace<camera_bounds_init>(entity);
            registry.emplace<camera_bounds>(entity);
            registry.emplace<camera_drag>(entity);
            registry.emplace<camera_zoom>(entity);

            return entity;
        }
    };
}