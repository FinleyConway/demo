#pragma once

#include "ecs/features/camera/components/camera_bounds.hpp"
#include "ecs/features/camera/components/camera_drag.hpp"
#include "ecs/features/camera/components/camera_zoom.hpp"

#include "ecs/features/camera/systems/camera_zooming.hpp"
#include "ecs/features/camera/systems/camera_dragging.hpp"
#include "ecs/features/camera/systems/setup_camera_bounds.hpp"

namespace demo::ecs {
    struct camera_future {
        using components = entt::type_list<
            camera_bounds, 
            camera_drag, 
            camera_zoom
        >;

        //using events = entt::type_list<>;
        
        using systems = entt::type_list<
            setup_camera_bounds, 
            camera_zooming, 
            camera_dragging
        >;
    };
}