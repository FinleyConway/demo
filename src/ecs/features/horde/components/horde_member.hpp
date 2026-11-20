#pragma once

#include <cstddef>

#include <entt/entt.hpp>

namespace demo::ecs {
    struct horde_member {
        entt::entity horde = entt::null;
        size_t path_index = 0;
    };
}