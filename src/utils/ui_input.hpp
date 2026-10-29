#pragma once

#include "input.hpp"

namespace demo {
    // consumes input to prevent gameplay input from fighting
    class ui_input {
    private:
        using mouse = input::mouse;

    public:
        static bool mouse_button_pressed(mouse button) {
            input::consume_mouse_button(button);    

            return IsMouseButtonPressed(input::to_raylib(button));
        }

        static bool mouse_button_held(mouse button) {
            input::consume_mouse_button(button);

            return IsMouseButtonDown(input::to_raylib(button));
        }

        static float mouse_scroll_delta() {
            input::consume_mouse_scroll();

            return GetMouseWheelMove();
        }

        static point mouse_position() {
            return input::mouse_position();
        }
    };
}