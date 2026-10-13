#pragma once

#include <cmath>
#include <algorithm>

namespace demo {
    struct point {
        float x = 0.0f;
        float y = 0.0f;

        constexpr point lerp_to(const point& target, float time) const {
            float clamp_time = std::clamp(time, 0.0f, 1.0f);

            return {
                std::lerp(x, target.x, clamp_time),
                std::lerp(y, target.y, clamp_time)
            };
        }
    };
}