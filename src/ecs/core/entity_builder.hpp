#pragma once

#include <entt/entt.hpp>

#include "resources/asset_context.hpp"

namespace demo::ecs {
    class entity_builder {
    public:
        virtual ~entity_builder() = default;

    public:
        virtual void build(entt::registry& registry, const asset_context& ctx) = 0;
    };
}