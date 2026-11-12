#pragma once

#include <cstdint>

#include <nlohmann/json.hpp>

#include "core/math/point.hpp"

namespace demo {
    template<typename T>
    struct rect_generic {
        T x = 0;
        T y = 0;
        T w = 0;
        T h = 0;

        constexpr bool in_bounds(const point& p) const {
            return p.x >= x     &&
                   p.y >= y     &&
                   p.x <  x + w &&
                   p.y <  y + h;
        }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(rect_generic,
            x, y, w, h
        );
    };

    using rect = rect_generic<float>;
    using rect_int = rect_generic<int32_t>;
}