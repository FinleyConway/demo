#pragma once

#include "ecs/core/system_base.hpp"
#include "ecs/components/navigation.hpp"
#include "ecs/components/positional.hpp"

#include "world/path_finding/path_finder.hpp"

namespace demo::ecs {
    class navigation_results : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<
                world_position,
                navigation_wait,
                navigation_path,
                navigation_request_state,
                navigation_motion
            >();

            // queries every frame for a generated path when having navigation_wait
            for (auto [entity, pos, path, request_state, motion] : view.each()) {
                path_finder& finder = context<path_finder>();

                if (finder.try_get_result(request_state.request, path.cells)) {
                    // reset motion for new path
                    motion.elapsed = 0.0f;
                    motion.start_position = pos.get();
                }
            }
        }
    };
}