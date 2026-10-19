#pragma once

#include <cstddef>

#include "resource/animation_clip.hpp"

namespace demo::ecs {
    struct animator {
        const animation_clip* clip = nullptr;

        size_t current_frame = 0;
        
        float time = 0.0f;
        float speed = 1.0f;
    };
}