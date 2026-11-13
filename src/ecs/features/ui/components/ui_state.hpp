#pragma once

#include <nlohmann/json.hpp>

#include "core/math/rect.hpp"

namespace demo::ecs {
    struct ui_sprite_states {
        rect_int idle_source;
        rect_int hover_source;
        rect_int pressed_source;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(ui_sprite_states, 
            idle_source,
            hover_source,
            pressed_source
        );
    };

    struct ui_pointer_capture {};

    struct ui_disabled {};

    struct ui_interactable {};

    struct ui_interaction_state {
        enum class state {
            idle,
            hover,
            press,
        };

        state value = state::idle;
    };

    struct ui_state_changed {
        enum class event {
            hover,
            unhover,
            press,
            release
        };

        event value;
    };
}