#pragma once

#include <vector>

#include <entt/entt.hpp>

#include "world/path_finding/movement_profile.hpp"

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
    
        void set_navigation_type(navigation_type cost) {
            m_navigation_type = cost;
        }

        navigation_type get_navigation_type() const {
            return m_navigation_type;
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

        navigation_type m_navigation_type = navigation_type::none;
    };
}