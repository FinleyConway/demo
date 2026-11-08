#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/ui/components/ui_state.hpp"
#include "ecs/features/ui/components/ui_sprite.hpp"

namespace demo::ecs {
    class ui_visual_state : public system_base {
    public:
        using system_base::system_base;

        void on_hook() override {
            registry().on_construct<ui_state_changed>()
                .connect<&ui_visual_state::on_state_change>(*this);

            registry().on_update<ui_state_changed>()
                .connect<&ui_visual_state::on_state_change>(*this);
        }

    private:
        void on_state_change(entt::registry& registry, entt::entity e) {
            ASSERT(
                (registry.all_of<ui_sprite_states, ui_sprite>(e)),
                "UI element missing sprite state components!"
            );

            const auto& state = registry.get<ui_interaction_state>(e);
            const auto& states = registry.get<ui_sprite_states>(e);
            auto& sprite = registry.get<ui_sprite>(e);

            switch (state.value) {
                case ui_interaction_state::state::idle:
                    sprite.source = states.idle_source;
                    sprite.tint = colour::white;
                    break;

                case ui_interaction_state::state::hover:
                    sprite.source = states.hover_source;
                    sprite.tint = colour::green;
                    break;

                case ui_interaction_state::state::press:
                    sprite.source = states.pressed_source;
                    sprite.tint = colour::blue;
                    break;
            }
        }
    };
}