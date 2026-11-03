#pragma once

#include "core/math/rect.hpp"
#include "ecs/core/prefab.hpp"

namespace demo::ecs {
    struct map_control {};

    struct defence_placer {
        prefab_id wall_prefab;
        prefab_id turret_prefab;
    };

    struct base_defence_zone {
        rect_int bpunds;
    };
}