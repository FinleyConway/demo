#pragma once

#include "world/world.hpp"
#include "ecs/features/camera/camera.hpp"

namespace demo::ecs {
    struct camera_spec {
        float zoom_increments = 0.35f;
        float zoom_min = 1.0f;
        float zoom_max = 4.0f;
    };

    struct camera_feature {
        static void register_systems(world& world) {
            world.add_system<setup_camera_bounds>();
            world.add_system<camera_zooming>();
            world.add_system<camera_dragging>();
        }

        static void make_camera(entt::registry& registry, entt::entity entity, const camera_spec& spec) {
            registry.emplace<camera_bounds_init>(entity);
            registry.emplace<camera_bounds>(entity);
            registry.emplace<camera_drag>(entity);
            registry.emplace<camera_zoom>(entity, camera_zoom {
                .increments = spec.zoom_increments,
                .min = spec.zoom_min,
                .max = spec.zoom_max,
            });
        }
    };
}