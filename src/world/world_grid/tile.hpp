#pragma once

#include <entt/entt.hpp>

#include "world/path_finding/movement_profile.hpp"

namespace demo {
    struct tile {
        navigation_type navigation_type = navigation_type::none;
        entt::entity structure = entt::null;
        entt::entity entity = entt::null;

        bool has_structure(const entt::registry& registry) const {
            return registry.valid(structure);
        }

        bool has_entity(const entt::registry& registry) const {
            return registry.valid(entity);
        }
    };
}