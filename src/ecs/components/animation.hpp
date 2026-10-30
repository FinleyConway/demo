#pragma once

#include <chrono>
#include <cstddef>

#include "resources/types/animation_clip.hpp"

namespace demo::ecs {
    struct animator {
        const animation_clip* clip = nullptr;

        size_t current_frame = 0;
        
        std::chrono::milliseconds time;
        float speed = 1.0f;
    };
}