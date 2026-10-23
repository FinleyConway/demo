#pragma once

#include <entt/entt.hpp>

#include "grid/grid.hpp"

namespace demo {
    struct tile {
    public:
        void set_entity(entt::entity entity) {
            m_entity = entity;
        }

        bool empty() const {
            return m_entity == entt::null;
        }
    
    private:
        entt::entity m_entity = entt::null;
    };
    
    using world_grid = grid<tile, 30, 30, 32>;
}