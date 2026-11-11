#pragma once

#include "ecs/features/positioning/components/dirty_position.hpp"
#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/positioning/components/is_static.hpp"

#include "ecs/features/positioning/systems/apply_position.hpp"

#include "world/world.hpp"

namespace demo::ecs {
    struct positioning_feature {
        static void register_to(world& world) {
            world.register_component<is_static>();
            world.register_component<position>();

            world.register_system<apply_position>();
        }
    };
}