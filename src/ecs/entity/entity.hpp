#pragma once

#include <entt/entt.hpp>

namespace demo::ecs {
    template<typename TDerived>
    struct entity {
        template<typename... Args>
        static entt::entity create(entt::registry& registry, Args&&... args) {
            auto handle = registry.create();

            TDerived::add_components(
                registry,
                handle,
                std::forward<Args>(args)...
            );

            return handle;
        }
    };
}