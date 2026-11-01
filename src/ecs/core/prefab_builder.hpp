#pragma once

#include <entt/entt.hpp>

#include "resources/asset_context.hpp"

namespace demo::ecs {
    class prefab_builder {
    public:
        virtual ~prefab_builder() = default;

    public:
        virtual entt::entity build(entt::registry& registry, const asset_context& ctx) = 0;
    };
}