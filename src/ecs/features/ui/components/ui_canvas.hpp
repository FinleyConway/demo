#pragma once

#include <entt/entt.hpp>

namespace demo::ecs {
    struct ui_element { };

    struct ui_canvas { };

    struct ui_canvas_capture {
        entt::entity entity = entt::null;
    };
}