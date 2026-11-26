#include "pch.hpp"
#include "ecs/core/prefab_blueprint.hpp"

namespace demo::ecs {
    prefab_blueprint::prefab_blueprint(entt::registry& registry, entt::registry& prefab_registry) :
        m_registry(registry),
        m_prefab_registry(prefab_registry)
    {
    } 

    entt::entity prefab_blueprint::instantiate() {
        auto entity = m_registry.create();

        setup_prefab(m_registry, entity);

        return entity;
    }

    void prefab_blueprint::update(std::vector<instruction>&& instructions) {
        m_prefab = m_prefab_registry.create();
        m_instructions = std::move(instructions);

        setup_prefab(m_prefab_registry, m_prefab);
    }

    void prefab_blueprint::setup_prefab(entt::registry& registry, entt::entity e) const {
        for (const auto& instruction : m_instructions) {
            instruction(registry, e);
        }
    }
}