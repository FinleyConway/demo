#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/ui/components/ui_state.hpp"

#include "core/input/ui_input.hpp"

namespace demo::ecs {
    class ui_click : public system_base {
    public:
        using system_base::system_base;
    
        void update(float dt) {
            auto view = registry().view<
                ui_interactable,
                ui_interaction_state
            >(entt::exclude<ui_disabled>);

            for (auto [e, state] : view.each()) {
                handle_click(e, state);
            }
        }

    private:
        void handle_click(entt::entity e, ui_interaction_state& state) {
            if (registry().all_of<ui_pointer_capture>(e)) {
                ui_input::consume_mouse_input();
            }

            if (state.value == ui_interaction_state::state::hover &&
                ui_input::mouse_button_pressed(input::mouse::left)) {

                registry().emplace<ui_pointer_capture>(e);

                set_state(e, state, ui_interaction_state::state::press);

                ui_input::consume_mouse_input();
            }

            if (state.value == ui_interaction_state::state::press &&
                ui_input::mouse_button_release(input::mouse::left)) {
                
                registry().remove<ui_pointer_capture>(e);

                set_state(e, state, ui_interaction_state::state::idle);
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