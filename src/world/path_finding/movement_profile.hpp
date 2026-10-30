#pragma once

#include <limits>
#include <cstddef>

namespace demo {
    enum class navigation_type {
        wall,
    };

    struct movement_cost {
        size_t wall = 1;
        
        constexpr size_t cost_for(navigation_type type) const {
            switch (type) {
                case navigation_type::wall: return wall;
            }

            return std::numeric_limits<size_t>::max();
        }
    };
}