#pragma once

#include "ecs/features/entity_init/components/entity_spawned.hpp"

#include "ecs/features/entity_init/systems/entity_spawn_register.hpp"

#include "world/world.hpp"

namespace demo::ecs {
    struct entity_init_feature {
        void register_to(world& world) {
            world.register_component<entity_spawned>();

            
            world.register_system<entity_spawn_register>();
        }
    };
}