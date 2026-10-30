#pragma once

#include <limits>
#include <cstddef>

namespace demo {
    enum class navigation_type {
        none,
        turret,
        wall,
    };

    struct movement_cost {
        size_t turret = 1;
        size_t wall = 1;
        
        constexpr size_t cost_for(navigation_type type) const {
            switch (type) {
                case navigation_type::none: return 1;
                case navigation_type::turret: return turret;
                case navigation_type::wall: return wall;
            }

            return std::numeric_limits<size_t>::max();
        }
    };
}