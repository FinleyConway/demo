#pragma once

#include "world_grid.hpp"
#include "entity/enemy.hpp"
#include "components/path_finder.hpp"

namespace demo {
    class world {
    public:
        world() : 
            m_finder(m_grid),
            m_enemy(m_grid, m_finder)
        {
        }
    
    public:
        void update(float dt) {
            m_enemy.update(dt);
        }

        void draw() {
            m_enemy.render();
        }

    private:
        world_grid m_grid;
        path_finder m_finder;

        enemy m_enemy;
    };
}