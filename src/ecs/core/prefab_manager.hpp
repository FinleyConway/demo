#pragma once

#include <unordered_map>

#include <entt/entt.hpp>

#include "core/debugging.hpp"
#include "ecs/core/prefab.hpp"

namespace demo::ecs {
    using prefab_id = uint32_t;

    class prefab_manager {
    public:
        explicit prefab_manager(entt::registry& registry) :
            m_registry(registry)
        {
        }

    public:
        template<typename T>
        const T* peek(prefab_id id) const {
            const auto it = m_prefabs.find(id);

            ASSERT(it != m_prefabs.end(), "Recieved a invalid prefab_id!");

            return it->second.peek<T>();
        }

        entt::entity instantiate(prefab_id id) const {
            const auto it = m_prefabs.find(id);

            ASSERT(it != m_prefabs.end(), "Recieved a invalid prefab_id!");

            return it->second.instantiate();
        }

    private:
        prefab_id create(std::vector<prefab::instruction>&& instructions) {
            const prefab_id id = m_next_id++;

            auto [it, inserted] = m_prefabs.try_emplace(
                id,
                m_registry,
                m_prefab_registry,
                std::move(instructions)
            );

            ASSERT(inserted);

            return id;
        }

    private:
        friend class entity_loader;

    private:
        entt::registry m_prefab_registry;
        entt::registry& m_registry;

        prefab_id m_next_id = 1;
        std::unordered_map<prefab_id, prefab> m_prefabs;
    };
}