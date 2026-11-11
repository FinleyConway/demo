#pragma once 

#include <entt/entt.hpp>

namespace demo::ecs {
    class prefab {
    private:
        using instruction = std::function<void(entt::registry&, entt::entity)>;
    
    public:
        prefab(entt::registry& registry, entt::registry& prefab_registry) :
            m_registry(registry),
            m_prefab_registry(prefab_registry)
        {
        } 

    public:
        template<typename T>
        const T* peek() const {
            return m_prefab_registry.try_get<T>(m_prefab);
        }

        entt::entity instantiate() const {
            auto entity = m_registry.create();

            setup_prefab(m_registry, entity);

            return entity;
        }

        void update(std::vector<instruction>&& instructions) {
            m_prefab = m_prefab_registry.create();

            setup_prefab(m_prefab_registry, m_prefab);
        }

    private:
        void setup_prefab(entt::registry& registry, entt::entity e) const {
            for (const auto& instruction : m_instructions) {
                instruction(registry, e);
            }
        }

    private:
        friend class prefab_manager;

        entt::registry& m_registry;
        entt::registry& m_prefab_registry;

        entt::entity m_prefab = entt::null;
        std::vector<instruction> m_instructions;
    };
}