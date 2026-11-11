#pragma once

#include <vector>

#include <entt/entt.hpp>

#include "core/debugging.hpp"
#include "ecs/core/prefab.hpp"

namespace demo::ecs {
    using prefab_id = size_t;

    class prefab_manager {
    public:
        explicit prefab_manager(entt::registry& registry) :
            m_registry(registry)
        {
        }

    public:
        template<typename T>
        const T* peek(prefab_id id) const {
            ASSERT(id <= m_prefabs.size(), "Recieved a invalid prefab_id!");

            return m_prefabs[id].peek<T>();
        }

        entt::entity instantiate(prefab_id id) const {
            ASSERT(id <= m_prefabs.size(), "Recieved a invalid prefab_id!");

            return m_prefabs[id].instantiate();
        }

    private:
        prefab_id create(std::vector<prefab::instruction>&& instructions) {
            m_prefabs.emplace_back(
                m_registry, 
                m_prefab_registry, 
                std::move(instructions)
            );

            return m_prefabs.size() - 1;
        }

        prefab_id reserve_prefab() {
            m_prefabs.emplace_back(
                m_registry, 
                m_prefab_registry
            );

            return m_prefabs.size() - 1;
        }

        void update_reserved(prefab_id id, std::vector<prefab::instruction>&& instructions) {
            ASSERT(id <= m_prefabs.size(), "Recieved a invalid prefab_id!");

            m_prefabs[id].update(std::move(instructions));
        }

    private:
        friend class entity_loader;
        friend class prefab_finder;

        entt::registry m_prefab_registry;
        entt::registry& m_registry;

        std::vector<prefab> m_prefabs;
    };
}