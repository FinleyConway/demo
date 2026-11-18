#include "pch.hpp"
#include "ecs/core/prefab_manager.hpp"

namespace demo::ecs {
    prefab_manager::prefab_manager(entt::registry& registry)
        : m_registry(registry) 
    {
    }

    entt::entity prefab_manager::instantiate(prefab_id id) {
        ASSERT(id < m_prefabs.size(), "Recieved a invalid prefab_id!");

        return m_prefabs[id].instantiate();
    }

    void prefab_manager::create(prefab_id id, std::vector<prefab::instruction>&& instructions) {
        ASSERT(id < m_prefabs.size(), "Recieved a invalid prefab_id!");

        m_prefabs[id].update(std::move(instructions));
    }

    prefab_id prefab_manager::reserve_prefab() {
        m_prefabs.emplace_back(m_registry, m_prefab_registry);

        return m_prefabs.size() - 1;
    }
} 