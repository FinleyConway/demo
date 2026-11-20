#include "pch.hpp"
#include "ecs/features/navigation/systems/requesting/request_navigation_path.hpp"

#include "ecs/features/navigation/components/requesting/navigation_request.hpp"
#include "ecs/features/navigation/components/requesting/navigation_state.hpp"

#include "world/world_grid/world_grid.hpp"
#include "world/path_finding/path_finder.hpp"

namespace demo::ecs {
    void request_navigation_path::on_hook() {
        registry().on_construct<navigation_target>()
            .connect<&request_navigation_path::on_request_path>(*this);
    }   

    void request_navigation_path::on_request_path(entt::registry& registry, entt::entity entity) {
        ASSERT(
            (registry.all_of<
                navigation_target, navigation_profile, navigation_request_state
            >(entity)),
            "Entity doesn't have required components to request a path!"
        );

        const world_grid& grid = context<world_grid>();
        path_finder& finder = context<path_finder>();

        auto [target, profile, request_state] = registry.get<
            navigation_target, navigation_profile, navigation_request_state
        >(entity);

        // send off a worker to generate a path
        request_state.request = finder.request(
            target.start,
            target.end,
            profile.costs
        );
        
        // tag to tell the entity to now wait for the request to generate
        registry.emplace_or_replace<navigation_wait>(entity);
    }
}