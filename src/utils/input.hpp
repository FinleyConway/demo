#pragma once

#include <raylib.h>

#include "math/point.hpp"

namespace demo {
    class input {
    public:
        enum class mouse {
            left,
            middle,
            right
        };

        static bool mouse_button_pressed(mouse button) {
            MouseButton but;
            
            if (button == mouse::left) but = MOUSE_BUTTON_LEFT;
            else if (button == mouse::right) but = MOUSE_BUTTON_RIGHT;
            else but = MOUSE_BUTTON_MIDDLE;

            return IsMouseButtonPressed(but);
        }

        static bool mouse_button_held(mouse button) {
            MouseButton but;
            
            if (button == mouse::left) but = MOUSE_BUTTON_LEFT;
            else if (button == mouse::right) but = MOUSE_BUTTON_RIGHT;
            else but = MOUSE_BUTTON_MIDDLE;

            return IsMouseButtonDown(but);
        }

        static float mouse_scroll_delta() {
            return GetMouseWheelMove();
        }

        static point mouse_position() {
            Vector2 position = GetMousePosition();

            return {
                position.x,
                position.y
            };
        }
    };
}