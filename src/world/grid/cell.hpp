#pragma once

#include <cstddef>
#include <cstdint>

namespace demo {
    template<typename T>
    struct cell_generic {
        T x = 0;
        T y = 0;

        constexpr bool operator==(const cell_generic& other) const {
            return x == other.x && y == other.y;
        } 
    };

    using cell = cell_generic<size_t>;
    using spatial_cell = cell_generic<int64_t>;
}

template<typename T>
struct std::hash<demo::cell_generic<T>> {
    size_t operator()(const demo::cell_generic<T>& c) const noexcept {
        // https://stackoverflow.com/a/55083395
        size_t hash = std::hash<T>()(c.x);
        hash <<= sizeof(size_t) * 4;
        hash ^= std::hash<T>()(c.y);

        return std::hash<size_t>()(hash);
    }
};