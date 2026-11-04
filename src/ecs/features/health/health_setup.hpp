#pragma once

#include "world/world.hpp"
#include "ecs/features/health/health.hpp"

namespace demo::ecs {
    struct health_feature {
        static void register_systems(world& world) {
            world.add_system<apply_healing>();
            world.add_system<apply_damage>();
            world.add_system<handle_death>();
        }
    };
}