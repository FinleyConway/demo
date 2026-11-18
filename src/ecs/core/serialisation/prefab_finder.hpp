#pragma once

#include <string>
#include <unordered_map>

#include "ecs/core/prefab_manager.hpp"

namespace demo::ecs {
    class prefab_finder {
    public:
        explicit prefab_finder(prefab_manager& prefab_manager);

        prefab_id get(const std::string& name);

    private:
        prefab_manager& m_prefab_manager;
        std::unordered_map<std::string, prefab_id> m_prefab_lookup;
    };
}