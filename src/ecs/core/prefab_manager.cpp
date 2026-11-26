#include "pch.hpp"
#include "ecs/core/prefab_manager.hpp"

namespace demo::ecs {
    prefab_manager::prefab_manager(entt::registry& registry)
        : m_registry(registry) 
    {
    }

    entt::entity prefab_manager::instantiate(prefab prefab) {
        const auto index = static_cast<size_t>(prefab);

        ASSERT(index < m_prefabs.size(), "Recieved a invalid prefab_id!");

        return m_prefabs[index].instantiate();
    }

    void prefab_manager::create(prefab prefab, std::vector<prefab_blueprint::instruction>&& instructions) {
        const auto index = static_cast<size_t>(prefab);

        ASSERT(index < m_prefabs.size(), "Recieved a invalid prefab_id!");

        m_prefabs[index].update(std::move(instructions));
    }

    prefab prefab_manager::reserve_prefab() {
        m_prefabs.emplace_back(m_registry, m_prefab_registry);

        return prefab{ m_prefabs.size() - 1 };
    }
} 