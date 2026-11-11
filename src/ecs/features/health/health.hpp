#pragma once

#include "ecs/features/health/components/health_events.hpp"
#include "ecs/features/health/components/health.hpp"

#include "ecs/features/health/systems/apply_damage.hpp"
#include "ecs/features/health/systems/apply_healing.hpp"
#include "ecs/features/health/systems/handle_death.hpp"

#include "world/world.hpp"

namespace demo::ecs {
    struct health_feature {
        static void register_to(world& world) {
            world.register_component<health>();
            world.register_component<damage>();

            world.register_system<apply_healing>();
            world.register_system<apply_damage>();
            world.register_system<handle_death>();
        }
    };
}