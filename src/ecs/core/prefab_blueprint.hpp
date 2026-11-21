#pragma once 

#include <vector>
#include <functional>

#include <entt/fwd.hpp>

namespace demo::ecs {
    class prefab_blueprint {
    private:
        using instruction = std::function<void(entt::registry&, entt::entity)>;
    
    public:
        prefab_blueprint(entt::registry& registry, entt::registry& prefab_registry);

    public:
        template<typename T>
        const T* peek() const {
            return m_prefab_registry.try_get<T>(m_prefab);
        }

        entt::entity instantiate();

        void update(std::vector<instruction>&& instructions);

    private:
        void setup_prefab(entt::registry& registry, entt::entity e) const;

    private:
        friend class prefab_manager;

        entt::registry& m_registry;
        entt::registry& m_prefab_registry;

        entt::entity m_prefab = entt::null;
        std::vector<instruction> m_instructions;
    };
}