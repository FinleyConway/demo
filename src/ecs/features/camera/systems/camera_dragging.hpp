#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/camera/components/camera_drag.hpp"
#include "ecs/features/camera/components/camera_bounds.hpp"

#include "core/input/input.hpp"
#include "rendering/camera.hpp"

namespace demo::ecs {
    class camera_dragging : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<
                camera_bounds,
                camera_drag
            >();

            for (auto [e, bounds, drag] : view.each()) {
                drag_camera(bounds, drag);
            }
        }

    private:
        point clamp_bounds(const camera_bounds& bounds, point position) {
            const point min_bounds = bounds.min;
            const point max_bounds = bounds.max;

            return {
                .x = std::clamp(position.x, min_bounds.x, max_bounds.x),
                .y = std::clamp(position.y, min_bounds.y, max_bounds.y)
            };
        }

        void drag_camera(const camera_bounds& bounds, camera_drag& drag) {
            const point mouse_posiiton = input::mouse_position();

            if (input::mouse_button_pressed(input::mouse::left)) {
                drag.last_mouse_position = mouse_posiiton;
            }

            if (input::mouse_button_held(input::mouse::left)) {
                camera& main_camera = context<camera>();     

                const point delta = mouse_posiiton - drag.last_mouse_position;
                const point position = main_camera.get_position();
                const float zoom = main_camera.get_zoom();
                const point drag_offset = {
                    .x = position.x - delta.x / zoom,
                    .y = position.y - delta.y / zoom,
                };

                main_camera.set_position(clamp_bounds(bounds, drag_offset));

                drag.last_mouse_position = mouse_posiiton;
            }
        }
    };
}