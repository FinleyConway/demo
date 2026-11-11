#pragma once

#include "ecs/features/navigation/components/navigation_motion.hpp"
#include "ecs/features/navigation/components/navigation_obstruction.hpp"
#include "ecs/features/navigation/components/navigation_path.hpp"
#include "ecs/features/navigation/components/navigation_weight.hpp"
#include "ecs/features/navigation/components/navigation.hpp"

#include "ecs/features/navigation/systems/follow_navigation_path.hpp"
#include "ecs/features/navigation/systems/get_navigation_results.hpp"
#include "ecs/features/navigation/systems/request_navigation_path.hpp"

#include "world/world.hpp"

namespace demo::ecs {
    struct navigation_feature {
        static void register_to(world& world) {
            world.register_component<navigation_motion>();
            world.register_component<navigation_path>();
            world.register_component<navigation_request_state>();
            world.register_component<navigation_profile>();


            world.register_system<request_navigation_path>();
            world.register_system<get_navigation_results>();
            world.register_system<follow_navigation_path>();
        }
    };
}