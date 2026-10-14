#pragma once

#include "world_grid.hpp"

namespace demo {
    class grid_entity {
    public:
        grid_entity() = default;

        virtual ~grid_entity() = default;

    public:
        virtual void update(world_grid& world, float dt) {};
 
        virtual void render() {};
    };
}