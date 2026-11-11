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
#include "ecs/core/entity_loader.hpp"

namespace demo::ecs {
    struct navigation_feature {
        static void register_components(entity_loader& loader) {
            loader.register_component<navigation_motion>();
            loader.register_component<navigation_path>();
            loader.register_component<navigation_request_state>();
            loader.register_component<navigation_profile>();
        }

        static void register_systems(world& world) {
            world.add_system<request_navigation_path>();
            world.add_system<get_navigation_results>();
            world.add_system<follow_navigation_path>();
        }
    };
}