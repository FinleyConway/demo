#pragma once

#include "ecs/features/combat/components/area_attack.hpp"
#include "ecs/features/combat/components/attack_capabilities.hpp"
#include "ecs/features/combat/components/attack_pattern.hpp"
#include "ecs/features/combat/components/attack_target.hpp"
#include "ecs/features/combat/components/attack_type.hpp"
#include "ecs/features/combat/components/attack.hpp"

#include "ecs/features/combat/systems/acquire_attack_target.hpp"
#include "ecs/features/combat/systems/attack_obstruction.hpp"
#include "ecs/features/combat/systems/execute_attack.hpp"
#include "ecs/features/combat/systems/generate_attack_area.hpp"
#include "ecs/features/combat/systems/perform_instant_attack.hpp"

#include "world/world.hpp"

namespace demo::ecs {
    struct combat_feature {
        static void register_to(world& world) {
            world.register_component<attack_area_init>();
            world.register_component<attack_area>();
            world.register_component<attack_pattern>();
            world.register_component<attack_preference>();
            world.register_component<attack_cooldown>();
            world.register_component<instant_attack>();

            
            world.register_system<generate_attack_area>();

            // init attacks
            world.register_system<acquire_attack_target>();
            world.register_system<attack_obstruction>();

            // attack cooldowns
            world.register_system<execute_attack>();

            // attack types
            world.register_system<perform_instant_attack>();
        }
    };
}