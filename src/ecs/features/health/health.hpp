#pragma once

#include "ecs/features/health/components/damage.hpp"
#include "ecs/features/health/components/heal.hpp"
#include "ecs/features/health/components/health.hpp"

#include "ecs/features/health/systems/apply_damage.hpp"
#include "ecs/features/health/systems/apply_healing.hpp"

namespace demo::ecs {
    struct health_feature {
        using components = entt::type_list<
            health,
            heal_providing,
            damage_dealing
        >;

        using events = entt::type_list<
            heal,
            take_damage,
            killed
        >;
        
        using systems = entt::type_list<
            apply_healing,
            apply_damage
        >;
    };
}