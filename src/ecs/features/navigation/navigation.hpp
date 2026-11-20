#pragma once

#include <entt/core/type_traits.hpp>

#include "ecs/features/navigation/components/requesting/navigation_state.hpp"
#include "ecs/features/navigation/components/requesting/navigation_request.hpp"

#include "ecs/features/navigation/systems/requesting/get_navigation_results.hpp"
#include "ecs/features/navigation/systems/requesting/request_navigation_path.hpp"

namespace demo::ecs {
    struct navigation_feature {
        using components = entt::type_list<
            navigation_profile,
            navigation_request_state
        >;

        using events = entt::type_list<
            navigation_target,
            navigation_failed
        >;

        using systems = entt::type_list<
            request_navigation_path,
            get_navigation_results
        >;
    };
}