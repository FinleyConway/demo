#pragma once

#include <cstdint>

namespace demo::ecs {
    struct attack_pattern {
        enum class shape {
            none,
            circle,
            sector
        };
        
        shape attack_shape = shape::circle;
        float radius = 0.0f;
        uint16_t angle = 0; // sector direction in degrees
        uint16_t width = 0; // sector width in degrees
    };
}