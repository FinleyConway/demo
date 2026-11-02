#pragma once

#include <cstdint>

namespace demo {
    struct colour {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t a = 255;

        static const colour black;
        static const colour white;
        static const colour red ;
        static const colour green;
        static const colour blue;
    };

    inline constexpr colour colour::black { 0,   0,   0,   255 };
    inline constexpr colour colour::white { 255, 255, 255, 255 };
    inline constexpr colour colour::red   { 255, 0,   0,   255 };
    inline constexpr colour colour::green { 0,   128, 0,   255 };
    inline constexpr colour colour::blue  { 0,   0,   255, 255 };
}