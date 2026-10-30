#pragma once

#include <vector>

#include <entt/entt.hpp>

#include "world/grid/grid.hpp"

namespace demo {
    struct tile {
    public:
        void set_structure(entt::entity entity) {
            m_structure_entity = entity;
        }

        entt::entity get_structure() const {
            return m_structure_entity;
        }

        bool has_structure(entt::registry& registry) const {
            return registry.valid(m_structure_entity);
        }
    
        void set_navigation_cost(size_t cost) {
            m_navigation_cost = cost;
        }

        size_t navigation_cost() const {
            return m_navigation_cost;
        }

        const std::vector<entt::entity>& entities_in_cell() const {
            return m_entities;
        }

        void add_entity(entt::entity entity) {
            if (has_entity(entity)) return;

            m_entities.emplace_back(entity);
        }

        void remove_entity(entt::entity entity) {
            std::erase(m_entities, entity);
        }

        bool has_entity(entt::entity entity) const {
            return std::ranges::find(m_entities, entity) != m_entities.end();
        }

    private:
        entt::entity m_structure_entity = entt::null;
        std::vector<entt::entity> m_entities;

        size_t m_navigation_cost = 1;
    };
    
    using world_grid = grid<tile, 30, 30, 32>;
}