#pragma once

#include "ecs/core/system_base.hpp"
#include "ecs/components/navigation.hpp"
#include "ecs/components/positional.hpp"

#include "world/world_grid/world_grid.hpp"
#include "world/path_finding/path_finder.hpp"

namespace demo::ecs {
    class navigation_request : public system_base {
    public:
        using system_base::system_base;

        void start() override {
            context<path_finder>().add_grid_context(context<world_grid>());
        }

        void update(float dt) override {
            auto view = registry().view<
                world_position,
                navigation_target,
                navigation_request_state
            >();

            // this only happens if navigation_target is added
            for (auto [e, pos, target, request_state] : view.each()) {
                const world_grid& grid = context<world_grid>();
                path_finder& finder = context<path_finder>();

                // request a path from starting position to target position
                const cell start_position = grid.point_to_cell(pos.get());

                request_state.request = finder.request(
                    start_position,
                    target.value
                );

                // remove target request
                registry().remove<navigation_target>(e);

                // tell it to start query for results
                registry().emplace_or_replace<navigation_wait>(e);
            }
        }
    };
}