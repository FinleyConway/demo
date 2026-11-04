#pragma once

#include "world/world.hpp"
#include "ecs/features/navigation/navigation.hpp"

namespace demo::ecs {
    struct navigation_feature {
        static void make_navigation(entt::registry& registry, entt::entity entity) {

        }

        static void register_systems(world& world) {
            world.add_system<request_navigation_path>();
            world.add_system<get_navigation_results>();
            world.add_system<follow_navigation_path>();
        }
    };
}