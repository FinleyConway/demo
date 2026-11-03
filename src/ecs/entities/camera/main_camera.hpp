#pragma once

#include "ecs/core/entity_builder.hpp"

#include "ecs/components/camera_control.hpp"

namespace demo::ecs {
    struct main_camera : entity_builder {
        entt::entity build(entt::registry& registry, const asset_context& ctx) override {
            auto entity = registry.create();
            
            registry.emplace<camera_init>(entity);
            registry.emplace<camera_control>(entity);

            return entity;
        }
    };
}