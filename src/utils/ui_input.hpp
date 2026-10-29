#pragma once

#include "input.hpp"

namespace demo {
    // consumes input to prevent gameplay input from fighting
    class ui_input {
    private:
        using mouse = input::mouse;

    public:
        static bool mouse_button_pressed(mouse button) {
            return IsMouseButtonPressed(input::to_raylib(button));
        }

        static bool mouse_button_held(mouse button) {
            return IsMouseButtonDown(input::to_raylib(button));
        }

        static float mouse_scroll_delta() {
            return GetMouseWheelMove();
        }

        static point mouse_position() {
            return input::mouse_position();
        }

        static void consume_mouse_input() {
            input::consume_mouse_button(mouse::left);
            input::consume_mouse_button(mouse::middle);   
            input::consume_mouse_button(mouse::right);   
            input::consume_mouse_scroll();   
        }

        static void consume_mouse_button(mouse button) {
            input::consume_mouse_button(button);   
        }

        static void consume_mouse_scroll() {
            input::consume_mouse_scroll();
        }
    };
}