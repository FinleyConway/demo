#pragma once

#include <entt/entt.hpp>

#include "ecs/core/event_cleanup.hpp"
#include "ecs/core/prefab_manager.hpp"

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
            ASSERT(
                (m_registry.ctx().get<event_cleanup>().is_tracked<T>()),
                "Event not registered in the feature's events list"
            );

            m_registry.emplace_or_replace<T>(e, std::forward<Args>(args)...);
        }

        entt::entity prefab_instantiate(prefab prefab) {
            return m_registry.ctx().get<prefab_manager>().instantiate(prefab);
        }

        template<typename T>
        const T* prefab_try_get(prefab prefab) const {
            return m_registry.ctx().get<prefab_manager>().peek<T>(prefab);
        }

    private:
        entt::registry& m_registry;
    };
}