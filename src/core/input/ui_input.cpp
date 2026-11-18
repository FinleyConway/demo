#include "pch.hpp"
#include "core/input/ui_input.hpp"

namespace demo {
    bool ui_input::mouse_button_pressed(mouse button) {
        return IsMouseButtonPressed(input::to_raylib(button));
    }

    bool ui_input::mouse_button_held(mouse button) {
        return IsMouseButtonDown(input::to_raylib(button));
    }

    bool ui_input::mouse_button_release(mouse button) {
        return IsMouseButtonUp(input::to_raylib(button));
    }

    float ui_input::mouse_scroll_delta() {
        return GetMouseWheelMove();
    }

    point ui_input::mouse_position() {
        return input::mouse_position();
    }

    void ui_input::consume_mouse_input() {
        input::consume_mouse_button(mouse::left);
        input::consume_mouse_button(mouse::middle);   
        input::consume_mouse_button(mouse::right);   
        input::consume_mouse_scroll();   
    }

    void ui_input::consume_mouse_button(mouse button) {
        input::consume_mouse_button(button);   
    }

    void ui_input::consume_mouse_scroll() {
        input::consume_mouse_scroll();
    }
}