#pragma once

#include <bitset>

#include <raylib.h>

#include "core/math/point.hpp"

namespace demo {
    class ui_input;

    class input {
    public:
        enum class mouse {
            left,
            middle,
            right
        };

    public:
        void begin_frame();

    public:
        static bool mouse_button_pressed(mouse button);

        static bool mouse_button_held(mouse button);

        static bool mouse_button_release(mouse button);

        static float mouse_scroll_delta();

        static point mouse_position();

    private:
        static MouseButton to_raylib(mouse button);

        static bool is_consumed(mouse button);

        static void consume_mouse_button(mouse button);

        static void consume_mouse_scroll();

    private:
        friend class ui_input;

        static inline std::bitset<3> s_consumed{};
        static inline bool s_scroll_consumed = false;
    };
}