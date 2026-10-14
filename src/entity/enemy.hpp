#pragma once

#include "entity/grid_entity.hpp"

#include "components/path_finder.hpp"
#include "components/auto_navigator.hpp"

namespace demo {
    class enemy : public grid_entity {
    public:
        enemy(world_grid& grid, path_finder& path_finder) :
            m_path_finder(path_finder),
            m_navigator(grid, m_path_finder)
        {
        }

    public:
        void update(float dt) {
            m_navigator.update(dt);
        }

    private:
        path_finder& m_path_finder; // global component

        auto_navigator m_navigator;
    };
}