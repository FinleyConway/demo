#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/ui/components/ui_state.hpp"
#include "ecs/features/ui/components/ui_canvas.hpp"

#include "core/input/ui_input.hpp"
#include "ecs/features/ui/ui_utils.hpp"

namespace demo::ecs {
    class ui_click : public system_base {
    public:
        using system_base::system_base;
    
        void update(float dt) {
            auto view = registry().view<ui_canvas, ui_canvas_capture>();

            for (auto [e, _, capture] : view.each()) {
                check_interactables(e, capture);
            }
        }

    private:
        void check_interactables(entt::entity canvas, ui_canvas_capture& capture) {
            auto view = ui_utils::canvas_view<
                ui_interactable,
                ui_interaction_state
            >(registry(), canvas);

            for (auto e : view) {
                auto& state = registry().get<ui_interaction_state>(e);

                handle_click(e, state, capture);
            }
        }

        void handle_click(entt::entity e, ui_interaction_state& state, ui_canvas_capture& capture) {
            if (capture.entity != entt::null) {
                ui_input::consume_mouse_input();
            }

            if (state.value == ui_interaction_state::state::hover &&
                ui_input::mouse_button_pressed(input::mouse::left)) {

                capture.entity = e;

                set_state(e, state, ui_interaction_state::state::press);

                ui_input::consume_mouse_input();
            }

            if (state.value == ui_interaction_state::state::press &&
                ui_input::mouse_button_release(input::mouse::left)) {
                
                capture.entity = entt::null;

                set_state(e, state, ui_interaction_state::state::hover);
            }
        }

        void set_state(entt::entity e, ui_interaction_state& state, ui_interaction_state::state new_state) {
            if (state.value == new_state) return;

            state.value = new_state;

            registry().emplace_or_replace<ui_state_changed>(
                e,
                new_state == ui_interaction_state::state::hover
                    ? ui_state_changed::event::hover
                    : ui_state_changed::event::unhover
            );
        }
    };
}