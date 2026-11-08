#pragma once

#include "core/debugging.hpp"
#include "ecs/core/system_base.hpp"

#include "ecs/features/ui/components/ui_size.hpp"
#include "ecs/features/ui/components/ui_state.hpp"
#include "ecs/features/positioning/components/position.hpp"

#include "core/input/ui_input.hpp"

namespace demo::ecs {
    class ui_hover : public system_base {
    public:
        using system_base::system_base;
    
        void update(float dt) {
            auto view = registry().view<
                ui_interactable,
                ui_interaction_state,
                position,
                ui_size
            >(entt::exclude<ui_disabled, ui_pointer_capture>);

            for (auto [e, state, pos, size] : view.each()) {
                if (is_mouse_over(pos, size)) {
                    // make sure the mouse is being consumed while the mouse is over
                    ui_input::consume_mouse_input();

                    set_state(e, state, ui_interaction_state::state::hover);
                }
                else {
                    set_state(e, state, ui_interaction_state::state::idle);
                }
            }
        }

    private:
        bool is_mouse_over(const position& pos, const ui_size& size) {
            const point mouse_position = ui_input::mouse_position();

            const rect ui_bounds = {
                .x = pos.value.x,
                .y = pos.value.y,
                .w = size.value.x,
                .h = size.value.y,
            };

            return ui_bounds.in_bounds(mouse_position);
        }

        void set_state(entt::entity e, ui_interaction_state& state, ui_interaction_state::state new_state) {
            if (state.value == ui_interaction_state::state::press) return;
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