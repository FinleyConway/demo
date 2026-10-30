#pragma once

#include <entt/entt.hpp>

namespace demo::ecs {
    class system_base {
    public:
        explicit system_base(entt::registry& registry) :
            m_registry(registry)
        {
        }

        virtual ~system_base() = default;

    public:
        virtual void on_hook() {}

        virtual void start() {}

        virtual void update(float dt) {}

        virtual void end() {}

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

    private:
        entt::registry& m_registry;
    };
}