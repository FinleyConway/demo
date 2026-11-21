#pragma once

#include "ecs/core/prefab_manager.hpp"

namespace demo::ecs {
    struct structures {
        prefab core_prefab = prefab::null;
        prefab wall_prefab = prefab::null;
        prefab turret_prefab = prefab::null;
    };
}