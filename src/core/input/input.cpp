#include "pch.hpp"
#include "core/input/input.hpp"

namespace demo {
    void input::begin_frame() {
        s_consumed.reset();
        s_scroll_consumed = false;
    }

    bool input::mouse_button_pressed(mouse button) {
        return !is_consumed(button)
            && IsMouseButtonPressed(to_raylib(button));
    }

    bool input::mouse_button_held(mouse button) {
        return !is_consumed(button)
            && IsMouseButtonDown(to_raylib(button));
    }

    bool input::mouse_button_release(mouse button) {
        return !is_consumed(button)
            && IsMouseButtonUp(to_raylib(button));
    }

    float input::mouse_scroll_delta() {
        return s_scroll_consumed
            ? 0.0f
            : GetMouseWheelMove();
    }

    point input::mouse_position() {
        const Vector2 p = GetMousePosition();

        return { p.x, p.y };
    }

    MouseButton input::to_raylib(mouse button) {
        switch (button) {
            case mouse::left:   return MOUSE_BUTTON_LEFT;
            case mouse::middle: return MOUSE_BUTTON_MIDDLE;
            case mouse::right:  return MOUSE_BUTTON_RIGHT;
        }

        return MOUSE_BUTTON_LEFT;
    }

    bool input::is_consumed(mouse button) {
        return s_consumed.test(static_cast<size_t>(button));
    }

    void input::consume_mouse_button(mouse button) {
        s_consumed.set(static_cast<size_t>(button));
    }

    void input::consume_mouse_scroll() {
        s_scroll_consumed = true;
    }
}