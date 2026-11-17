#pragma once

#include "ecs/features/map/components/map.hpp"

#include "ecs/features/map/systems/create_map_visuals.hpp"
#include "ecs/features/map/systems/setup_map.hpp"

namespace demo::ecs {
    struct map_feature {
        using components = entt::type_list<
            map
        >;

        //using events = entt::type_list<>;
        
        using systems = entt::type_list<
            setup_map,
            create_map_visuals
        >;
    };
}