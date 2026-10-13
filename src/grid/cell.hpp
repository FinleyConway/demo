#pragma once

#include <cstddef>
#include <utility>
#include <typeindex> // std::hash

namespace demo {
    struct cell {
        size_t x = 0;
        size_t y = 0;

        constexpr bool operator==(const cell& other) const {
            return x == other.x && y == other.y;
        } 
    };
}

template<>
struct std::hash<demo::cell> {
    size_t operator()(const demo::cell& c) const noexcept {
        // https://stackoverflow.com/a/55083395
        size_t hash = std::hash<size_t>()(c.x);
        hash <<= sizeof(size_t) * 4;
        hash ^= std::hash<size_t>()(c.y);

        return std::hash<size_t>()(hash);
    }
};