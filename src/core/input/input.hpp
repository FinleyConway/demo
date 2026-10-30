#pragma once

#include <bitset>

#include <raylib.h>

#include "math/point.hpp"

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
        void begin_frame() {
            s_consumed.reset();
            s_scroll_consumed = false;
        }

    public:
        static bool mouse_button_pressed(mouse button) {
            return !is_consumed(button)
                && IsMouseButtonPressed(to_raylib(button));
        }

        static bool mouse_button_held(mouse button) {
            return !is_consumed(button)
                && IsMouseButtonDown(to_raylib(button));
        }

        static float mouse_scroll_delta() {
            return s_scroll_consumed
                ? 0.0f
                : GetMouseWheelMove();
        }

        static point mouse_position() {
            const Vector2 p = GetMousePosition();

            return { p.x, p.y };
        }

    private:
        static MouseButton to_raylib(mouse button) {
            switch (button) {
                case mouse::left:   return MOUSE_BUTTON_LEFT;
                case mouse::middle: return MOUSE_BUTTON_MIDDLE;
                case mouse::right:  return MOUSE_BUTTON_RIGHT;
            }

            return MOUSE_BUTTON_LEFT;
        }

        static bool is_consumed(mouse button) {
            return s_consumed.test(static_cast<size_t>(button));
        }

        static void consume_mouse_button(mouse button) {
            s_consumed.set(static_cast<size_t>(button));
        }

        static void consume_mouse_scroll() {
            s_scroll_consumed = true;
        }

    private:
        friend class ui_input;

        static inline std::bitset<3> s_consumed{};
        static inline bool s_scroll_consumed = false;
    };
}