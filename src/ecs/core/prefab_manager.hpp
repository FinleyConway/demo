#pragma once

#include <unordered_map>

#include <entt/entt.hpp>

#include "core/debugging.hpp"
#include "ecs/core/prefab.hpp"
#include "ecs/core/prefab_builder.hpp"

namespace demo {
    class world;
}

namespace demo::ecs {
    class prefab_manager {
    public:
        explicit prefab_manager(entt::registry& registry) :
            m_registry(registry) 
        {
        }

    public:
        template<typename T>
        const T* peak(prefab_id id) const {
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
        friend class demo::world;

        template<typename T>
        prefab_id create() {
            static_assert(std::derived_from<T, ecs::prefab_builder>);

            const prefab_id id = m_next_id++;

            auto [it, inserted] = m_prefabs.try_emplace(
                id,
                prefab(m_registry, m_prefab_registry, std::in_place_type<T>)
            );

            ASSERT(inserted);

            return id;
        }

    private:
        entt::registry m_prefab_registry;
        entt::registry& m_registry;

        size_t m_next_id = 0;
        std::unordered_map<prefab_id, prefab> m_prefabs;
    };
}