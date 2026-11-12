#pragma once

#include <chrono>
#include <cstddef>

#include "resources/asset_context.hpp"

namespace demo::ecs {
    struct animator {
        asset_id clip = invalid_asset_id;

        size_t current_frame = 0;
        
        std::chrono::milliseconds time;
        float speed = 1.0f;
    };
}   