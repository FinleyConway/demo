#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/navigation/components/navigation_path.hpp"
#include "ecs/features/navigation/components/navigation_state.hpp"

#include "world/path_finding/path_finder.hpp"

namespace demo::ecs {
    class get_navigation_results : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<
                navigation_wait,
                navigation_request_state
            >();
            
            path_finder& finder = context<path_finder>();

            // queries every frame for a generated path when having navigation_wait
            for (auto [e, request_state] : view.each()) {
                auto result = finder.try_get_result(request_state.request);
                
                // still searching this frame
                if (!result.has_value()) continue;

                registry().remove<navigation_wait>(e);

                if (result->empty()) registry().emplace<navigation_failed>(e);
                else registry().emplace_or_replace<navigation_path>(e, std::move(*result));
            }
        }
    };
}