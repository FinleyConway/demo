#pragma once

#include "ecs/features/map/components/map.hpp"

#include "ecs/features/map/systems/setup_map.hpp"

#include "world/world.hpp"
#include "ecs/core/entity_loader.hpp"

namespace demo::ecs {
    struct map_feature {
        static void register_components(entity_loader& loader) {
            loader.register_component<map_init>();
            loader.register_component<map>();
        }

        static void register_systems(world& world) {
            world.add_system<setup_map>();
        }
    };
}