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
#include "ecs/core/entity_loader.hpp"

namespace demo::ecs {
    struct combat_feature {
        static void register_components(entity_loader& loader) {
            loader.register_component<attack_area_init>();
            loader.register_component<attack_area>();
            loader.register_component<attack_pattern>();
            loader.register_component<attack_preference>();
            loader.register_component<attack_cooldown>();
            loader.register_component<instant_attack>();
        }

        static void register_systems(world& world) {
            world.add_system<generate_attack_area>();

            // init attacks
            world.add_system<acquire_attack_target>();
            world.add_system<attack_obstruction>();

            // attack cooldowns
            world.add_system<execute_attack>();

            // attack types
            world.add_system<perform_instant_attack>();
        }
    };
}