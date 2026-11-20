#pragma once

#include <entt/core/type_traits.hpp>

#include "ecs/features/horde/components/horde_handler.hpp"
#include "ecs/features/horde/components/horde_member.hpp"
#include "ecs/features/horde/systems/horde_finished.hpp"
#include "ecs/features/horde/systems/horde_member_killed.hpp"
#include "ecs/features/horde/systems/setup_horde.hpp"
#include "ecs/features/horde/systems/spawn_horde.hpp"

namespace demo::ecs {
    struct feature_horde {
        using components = entt::type_list<
            horde_info,
            horde_count,

            horde_member
        >;

        using events = entt::type_list<
            horde_target
        >;
        
        using systems = entt::type_list<
            setup_horde,
            spawn_horde,

            horde_member_killed,
            horde_finished
        >;
    };
}