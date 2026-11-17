#pragma once

#include "ecs/features/combat/components/attack_area.hpp"
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

namespace demo::ecs {
    struct combat_future {
        using components = entt::type_list<
            attack_area,
            attack_pattern,
            attack_preference,
            attack_cooldown,
            instant_attack
        >;

        using events = entt::type_list<
            attack
        >;
        
        using systems = entt::type_list<
            generate_attack_area,

            acquire_attack_target,
            attack_obstruction,

            execute_attack,

            perform_instant_attack
        >;
    };
}