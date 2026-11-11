#pragma once

#include "ecs/features/map/components/map.hpp"

#include "ecs/features/map/systems/setup_map.hpp"

#include "world/world.hpp"

namespace demo::ecs {
    struct map_feature {
        static void register_to(world& world) {
            world.register_component<map_init>();
            world.register_component<map>();

            world.register_system<setup_map>();
        }
    };
}