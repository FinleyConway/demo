#pragma once

#include <string>
#include <unordered_map>

#include "ecs/core/prefab_manager.hpp"

namespace demo::ecs {
    class prefab_finder {
    public:
        explicit prefab_finder(prefab_manager& prefab_manager) :
            m_prefab_manager(prefab_manager)
        {
        }

        prefab_id get(const std::string& name) {
            const auto it = m_prefab_lookup.find(name);

            // if the prefab hasn't been created yet
            if (it == m_prefab_lookup.end()) {
                // reserve a spot for this perfab
                const prefab_id id = m_prefab_manager.reserve_prefab();

                // create a lookup for the table with the name
                m_prefab_lookup.emplace(name, id);

                return id;
            }

            // return already reserved prefab
            return it->second;
        }

    private:
        prefab_manager& m_prefab_manager;
        std::unordered_map<std::string, prefab_id> m_prefab_lookup;
    };
}