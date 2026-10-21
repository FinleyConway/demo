#pragma once

#include <entt/entt.hpp>

#include "math/point.hpp"

namespace demo::ecs {
    struct static_entity {};

    struct dirty_position {};

    struct world_position {
    public:
        void set(point value, entt::registry& registry, entt::entity entity) {
            if (registry.all_of<static_entity>(entity)) return;
            if (m_value == value) return;

            m_value = value;
            
            if (!registry.all_of<dirty_position>(entity)) {
                registry.emplace<dirty_position>(entity);
            }
        }

        const point& get() const {
            return m_value;
        }

    private:
        point m_value;
    };
}