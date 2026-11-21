#pragma once

#include <vector>
#include <limits>

#include "core/debugging.hpp"
#include "ecs/core/prefab_blueprint.hpp"

namespace demo::ecs {
    enum class prefab : size_t {
        null = std::numeric_limits<std::size_t>::max()
    };

    class prefab_manager {
    public:
        explicit prefab_manager(entt::registry& registry);

    public:
        template<typename T>
        const T* peek(prefab prefab) const {
            const auto index = static_cast<size_t>(prefab);

            ASSERT(index < m_prefabs.size(), "Recieved a invalid prefab_id!");

            return m_prefabs[index].peek<T>();
        }

        entt::entity instantiate(prefab prefab);

    private:
        void create(prefab prefab, std::vector<prefab_blueprint::instruction>&& instructions);

        prefab reserve_prefab();

    private:
        friend class entity_loader;
        friend class prefab_finder;

        entt::registry m_prefab_registry;
        entt::registry& m_registry;

        std::vector<prefab_blueprint> m_prefabs;
    };
}