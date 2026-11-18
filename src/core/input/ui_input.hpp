#pragma once

#include "core/input/input.hpp"

namespace demo {
    // consumes input to prevent gameplay input from fighting
    class ui_input {
    private:
        using mouse = input::mouse;

    public:
        static bool mouse_button_pressed(mouse button);

        static bool mouse_button_held(mouse button);

        static bool mouse_button_release(mouse button);

        static float mouse_scroll_delta() ;

        static point mouse_position();

        static void consume_mouse_input();

        static void consume_mouse_button(mouse button);

        static void consume_mouse_scroll();
    };
}