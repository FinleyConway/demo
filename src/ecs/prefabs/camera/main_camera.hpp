#pragma once

#include "ecs/entity/entity.hpp"

#include "ecs/components/camera_control.hpp"

namespace demo::ecs {
    struct main_camera : entity<main_camera> {
        static void add_components(entt::registry& registry, entt::entity entity) {
            registry.emplace<camera_control>(entity);
        }
    };
}