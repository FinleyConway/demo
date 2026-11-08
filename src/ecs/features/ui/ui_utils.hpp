#pragma once

#include <ranges>

#include <entt/entt.hpp>

#include "ecs/features/ui/components/ui_canvas.hpp"
#include "ecs/features/ui/components/ui_state.hpp"

#include "core/debugging.hpp"

namespace demo::ecs {
    class ui_utils {
    public:
        template<typename... TComponents>
        static auto canvas_view(entt::registry& registry, entt::entity canvas) {
            ASSERT(
                (registry.all_of<ui_canvas>(canvas)),
                "Canvas doesn't have ui_canvas for view"
            );
            
            const canvas_id id = registry.get<ui_canvas>(canvas).id;
            auto view = registry.view<
                ui_element, 
                TComponents...
            >(entt::exclude<ui_disabled>);

            // filter ui_element entities that have the same id to canvas
            return view | std::views::filter([&registry, id](entt::entity entity) {
                return registry.get<ui_element>(entity).canvas == id;
            });
        }
    };
}