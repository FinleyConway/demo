#pragma once

#include <entt/entt.hpp>

#include "ecs/core/event_cleanup.hpp"
#include "ecs/core/prefab_manager.hpp"

#include "ecs/features/entity_init/components/entity_spawned.hpp"

namespace demo::ecs {
    enum class phase { start, input, update, render };

    class system_base {
    public:
        explicit system_base(entt::registry& registry) :
            m_registry(registry)
        {
        }

        virtual ~system_base() = default;

    public:
        virtual void on_hook() {}

        virtual void update(float dt) {}

    protected:
        entt::registry& registry() {
            return m_registry;
        }
    
        const entt::registry& registry() const {
            return m_registry;
        }

        template<typename T>
        T& context() {
            return m_registry.ctx().get<T>();
        }

        template<typename T>
        const T& context() const {
            return m_registry.ctx().get<T>();
        }

        template<typename T, typename... Args>
        void emit(entt::entity e, Args&&... args) {
            m_registry.ctx().get<event_cleanup>().track<T>();
            m_registry.emplace_or_replace<T>(e, std::forward<Args>(args)...);
        }

        entt::entity prefab_instantiate(prefab_id prefab) {
            // create prefab
            auto e = m_registry.ctx().get<prefab_manager>().instantiate(prefab);

            // emit spawn event
            emit<entity_spawned>(e);

            return e;
        }

        template<typename T>
        const T* prefab_try_get(prefab_id prefab) const {
            return m_registry.ctx().get<prefab_manager>().peek<T>(prefab);
        }

    private:
        entt::registry& m_registry;
    };
}