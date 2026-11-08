#pragma once

#include <entt/entt.hpp>

namespace demo::ecs {
    class system_base {
    public:
        explicit system_base(entt::registry& registry, entt::dispatcher& dispatcher) :
            m_registry(registry),
            m_dispatcher(dispatcher)
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

        entt::dispatcher& dispatcher() {
            return m_dispatcher;
        }
    
        const entt::dispatcher& dispatcher() const {
            return m_dispatcher;
        }

        template<typename T>
        T& context() {
            return m_registry.ctx().get<T>();
        }

        template<typename T>
        const T& context() const {
            return m_registry.ctx().get<T>();
        }

    private:
        entt::registry& m_registry;
        entt::dispatcher& m_dispatcher;
    };
}