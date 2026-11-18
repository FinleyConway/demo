#pragma once

#include <vector>

#include "core/debugging.hpp"
#include "ecs/core/prefab.hpp"

namespace demo::ecs {
    using prefab_id = size_t;

    class prefab_manager {
    public:
        explicit prefab_manager(entt::registry& registry);

    public:
        template<typename T>
        const T* peek(prefab_id id) const {
            ASSERT(id < m_prefabs.size(), "Recieved a invalid prefab_id!");

            return m_prefabs[id].peek<T>();
        }

        entt::entity instantiate(prefab_id id);

    private:
        void create(prefab_id id, std::vector<prefab::instruction>&& instructions);

        prefab_id reserve_prefab();

    private:
        friend class entity_loader;
        friend class prefab_finder;

        entt::registry m_prefab_registry;
        entt::registry& m_registry;

        std::vector<prefab> m_prefabs;
    };
}