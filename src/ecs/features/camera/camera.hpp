#pragma once

#include "ecs/features/camera/components/camera_bounds.hpp"
#include "ecs/features/camera/components/camera_drag.hpp"
#include "ecs/features/camera/components/camera_zoom.hpp"

#include "ecs/features/camera/systems/camera_zooming.hpp"
#include "ecs/features/camera/systems/camera_dragging.hpp"
#include "ecs/features/camera/systems/setup_camera_bounds.hpp"

#include "world/world.hpp"
#include "ecs/core/entity_loader.hpp"

namespace demo::ecs {
    struct camera_feature {
        static void register_systems(world& world) {
            world.add_system<setup_camera_bounds>();
            world.add_system<camera_zooming>();
            world.add_system<camera_dragging>();
        }

        static void register_components(entity_loader& loader) {
            loader.register_component<camera_bounds_init>();
            loader.register_component<camera_bounds>();
            loader.register_component<camera_drag>();
            loader.register_component<camera_zoom>();
        }
    };
}