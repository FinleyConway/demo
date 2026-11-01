#pragma once 

#include <memory>

#include <entt/entt.hpp>

#include "core/debugging.hpp"
#include "ecs/core/prefab_builder.hpp"
#include "resources/asset_context.hpp"

namespace demo::ecs {
    using prefab_id = size_t;

    class prefab {
    public:
        template<typename T>
        const T* peek() const {
            ASSERT(m_prefab_registry != nullptr);

            return m_prefab_registry->try_get<T>(m_prefab);
        }

        entt::entity instantiate() const {
            ASSERT(m_registry != nullptr);
            ASSERT(m_builder != nullptr);

            const auto& ctx = m_registry->ctx().get<asset_context>();

            return m_builder->build(*m_registry, ctx);
        }

    private:
        friend class prefab_manager;

        template<typename T>
        prefab(entt::registry& registry, entt::registry& prefab_registry, std::in_place_type_t<T>) :
            m_registry(&registry),
            m_prefab_registry(&prefab_registry),
            m_builder(std::make_unique<T>())
        {
            static_assert(std::derived_from<T, prefab_builder>);

            const auto& ctx = m_registry->ctx().get<asset_context>();

            m_prefab = m_builder->build(prefab_registry, ctx);
        } 

    private:
        entt::registry* m_registry = nullptr;
        entt::registry* m_prefab_registry = nullptr;

        entt::entity m_prefab = entt::null;

        std::unique_ptr<prefab_builder> m_builder;
    };
}