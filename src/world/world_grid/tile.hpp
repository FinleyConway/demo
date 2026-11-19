#pragma once

#include <array>

#include <entt/entt.hpp>

#include "entt/entity/entity.hpp"
#include "world/path_finding/movement_profile.hpp"

namespace demo {
    struct tile {
    public:
        tile() {
            m_entities.fill(entt::null);
        }
    
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
    
        void set_navigation_type(navigation_type cost) {
            m_navigation_type = cost;
        }

        navigation_type get_navigation_type() const {
            return m_navigation_type;
        }

        const std::array<entt::entity, 4>& entities_in_cell() const {
            return m_entities;
        }

        void add_entity(size_t quadrant, entt::entity entity) {
            if (quadrant >= m_entities.size()) return;
            if (has_entity(entity)) return;

            m_entities[quadrant] = entity;
        }

        void remove_entity(size_t quadrant) {
            if (quadrant >= m_entities.size()) return;

            m_entities[quadrant] = entt::null;
        }

        bool has_entity(entt::entity entity) const {
            return std::ranges::find(m_entities, entity) != m_entities.end();
        }

    private:
        entt::entity m_structure_entity = entt::null;
        std::array<entt::entity, 4> m_entities{};

        navigation_type m_navigation_type = navigation_type::none;
    };
}