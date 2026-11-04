#pragma once

#include <limits>
#include <cstdint>

namespace demo {
    enum class navigation_type {
        none,
        turret,
        wall,
    };

    struct movement_cost {
        uint32_t turret = 1;
        uint32_t wall = 1;
        
        constexpr uint32_t cost_for(navigation_type type) const {
            switch (type) {
                case navigation_type::none:   return minimum_cost();
                case navigation_type::turret: return turret;
                case navigation_type::wall:   return wall;
            }

            return std::numeric_limits<uint32_t>::max();
        }

        static constexpr uint32_t minimum_cost() {
            return 1;
        }
    };
}