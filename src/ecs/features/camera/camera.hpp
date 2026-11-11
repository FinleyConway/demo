#pragma once

#include "ecs/features/camera/components/camera_bounds.hpp"
#include "ecs/features/camera/components/camera_drag.hpp"
#include "ecs/features/camera/components/camera_zoom.hpp"

#include "ecs/features/camera/systems/camera_zooming.hpp"
#include "ecs/features/camera/systems/camera_dragging.hpp"
#include "ecs/features/camera/systems/setup_camera_bounds.hpp"

#include "world/world.hpp"

namespace demo::ecs {
    struct camera_feature {
        static void register_to(world& world) {
            world.register_component<camera_bounds_init>();
            world.register_component<camera_bounds>();
            world.register_component<camera_drag>();
            world.register_component<camera_zoom>();

            
            world.register_system<setup_camera_bounds>();
            world.register_system<camera_zooming>();
            world.register_system<camera_dragging>();
        }
    };
}