#pragma once

#include "ecs/features/positioning/components/dirty_position.hpp"
#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/positioning/components/is_static.hpp"

#include "ecs/features/positioning/systems/apply_position.hpp"

#include "world/world.hpp"
#include "ecs/core/entity_loader.hpp"

namespace demo::ecs {
    struct positioning_feature {
        static void register_components(entity_loader& loader) {
            loader.register_component<is_static>();
            loader.register_component<position>();
        }

        static void register_systems(world& world) {
            world.add_system<apply_position>();
        }
    };
}