#pragma once

#include <typeindex>

#include <entt/entt.hpp>

namespace demo {
    struct spatial_id {
        entt::entity entity;
        std::type_index type;

        bool operator==(const spatial_id&) const = default;
    };
}

template<>
struct std::hash<demo::spatial_id> {
    size_t operator()(const demo::spatial_id& id) const noexcept {
        const size_t h1 = std::hash<entt::entity>{}(id.entity);
        const size_t h2 = id.type.hash_code();

        return h1 ^ (h2 << 1);
    }
};