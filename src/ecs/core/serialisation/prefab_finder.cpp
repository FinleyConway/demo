#include "pch.hpp"
#include "ecs/core/serialisation/prefab_finder.hpp"

namespace demo::ecs {
    prefab_finder::prefab_finder(prefab_manager& prefab_manager) :
        m_prefab_manager(prefab_manager)
    {
    }

    prefab prefab_finder::get(const std::string& name) {
        const auto it = m_prefab_lookup.find(name);

        // if the prefab hasn't been created yet
        if (it == m_prefab_lookup.end()) {
            // reserve a spot for this perfab
            const prefab id = m_prefab_manager.reserve_prefab();

            // create a lookup for the table with the name
            m_prefab_lookup.emplace(name, id);

            return id;
        }

        // return already reserved prefab
        return it->second;
    }
}