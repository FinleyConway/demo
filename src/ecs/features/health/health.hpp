#pragma once

#include "ecs/features/health/components/health_events.hpp"
#include "ecs/features/health/components/health.hpp"

#include "ecs/features/health/systems/apply_damage.hpp"
#include "ecs/features/health/systems/apply_healing.hpp"
#include "ecs/features/health/systems/handle_death.hpp"

#include "ecs/core/entity_loader.hpp"
#include "world/world.hpp"

namespace demo::ecs {
    struct health_feature {
        static void register_components(entity_loader& loader) {
            loader.register_component<health>();
            loader.register_component<damage>();
        }

        static void register_systems(world& world) {
            world.add_system<apply_healing>();
            world.add_system<apply_damage>();
            world.add_system<handle_death>();
        }
    };
}