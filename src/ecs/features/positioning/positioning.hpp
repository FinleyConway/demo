#pragma once

#include "ecs/features/positioning/components/dirty_position.hpp"
#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/positioning/components/is_static.hpp"

#include "ecs/features/positioning/systems/apply_position.hpp"

namespace demo::ecs {
    struct positioning_feature {
        using components = entt::type_list<
            is_static,
            position
        >;

        using events = entt::type_list<
            dirty_position
        >;
        
        using systems = entt::type_list<
            apply_position
        >;
    };
}