#pragma once

#include "ecs/core/entity_builder.hpp"
#include "ecs/features/camera/camera_setup.hpp"

namespace demo::ecs {
    struct main_camera : entity_builder {
        entt::entity build(entt::registry& registry, const asset_context& ctx) override {
            auto entity = registry.create();
            
            camera_feature::make_camera(registry, entity, {
                .zoom_increments = 0.35f,
                .zoom_min = 1.0f,
                .zoom_max = 4.0f,
            });

            return entity;
        }
    };
}