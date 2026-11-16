#pragma once

#include "ecs/features/health/components/damage.hpp"
#include "ecs/features/health/components/heal.hpp"
#include "ecs/features/health/components/health.hpp"

#include "ecs/features/health/systems/manipulate_health.hpp"

#include "world/world.hpp"

namespace demo::ecs {
    struct health_feature {
        static void register_to(world& world) {
            world.register_component<health>();
            world.register_component<damage>();
            world.register_component<heal>();

            world.register_system<manipulate_health>();
        }
    };
}