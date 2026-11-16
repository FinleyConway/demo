#pragma once

#include <entt/entt.hpp>

namespace demo::ecs {
    struct killed_event {
        entt::entity entity;
    };
}