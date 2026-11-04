#pragma once

#include "world/world.hpp"
#include "ecs/features/map/map.hpp"

namespace demo::ecs {
    struct map_feature {
        static void make_map(entt::registry& registry, entt::entity entity) {
            registry.emplace<map_init>(entity);
            registry.emplace<map>(entity);
        }

        static void register_systems(world& world) {
            world.add_system<setup_map>();
        }
    };
}