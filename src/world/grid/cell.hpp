#pragma once

#include <cstdint>

namespace demo {
    struct cell {
        int32_t x = 0;
        int32_t y = 0;

        constexpr bool operator<=>(const cell& other) const = default;

        constexpr cell operator+(const cell& other) const {
            return {
                .x = x + other.x,
                .y = y + other.y
            };
        }        

        constexpr cell operator-(const cell& other) const {
            return {
                .x = x - other.x,
                .y = y - other.y
            };
        }       
    };
}

template<>
struct std::hash<demo::cell> {
    size_t operator()(const demo::cell& c) const noexcept {
        // https://stackoverflow.com/a/55083395
        size_t hash = std::hash<int32_t>()(c.x);
        hash <<= sizeof(int32_t) * 4;
        hash ^= std::hash<int32_t>()(c.y);

        return std::hash<int32_t>()(hash);
    }
};