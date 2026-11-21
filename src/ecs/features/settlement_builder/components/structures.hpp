#pragma once

#include "ecs/core/prefab_manager.hpp"

namespace demo::ecs {
    struct structures {
        prefab core_prefab;
        prefab wall_prefab;
        prefab turret_prefab;
    };
}