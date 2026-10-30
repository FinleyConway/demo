#pragma once

#include <cstdint>

namespace demo {
    template<typename T>
    struct rect_generic {
        T x = 0;
        T y = 0;
        T w = 0;
        T h = 0;
    };

    using rect = rect_generic<float>;
    using rect_int = rect_generic<int32_t>;
}