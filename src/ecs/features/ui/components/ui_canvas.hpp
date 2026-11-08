#pragma once

#include <cstdint>

#include <entt/entt.hpp>

namespace demo::ecs {
    using canvas_id = uint32_t;

    inline canvas_id generate_canvas_id() {
        static canvas_id next_id = 1;
        return next_id++;
    }

    struct ui_element {
        canvas_id canvas = 0;
    };

    struct ui_canvas {
        canvas_id id = generate_canvas_id();
    };

    struct ui_canvas_capture {
        entt::entity entity = entt::null;
    };
}