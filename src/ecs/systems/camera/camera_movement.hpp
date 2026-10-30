#pragma once

#include "ecs/core/system_base.hpp"
#include "ecs/components/camera_control.hpp"

#include "core/input/input.hpp"
#include "rendering/camera.hpp"
#include "world/grid/world_grid.hpp"

namespace demo::ecs {
    class camera_movement : public system_base {
    public:
        using system_base::system_base;

        void start() override {
            auto view = registry().view<camera_control>();

            for (auto [e, control] : view.each()) {
                setup_camera(control);
            }
        }

        void update(float dt) override {
            auto view = registry().view<camera_control>();

            for (auto [e, control] : view.each()) {
                zoom_camera(control);
                drag_camera(control);
            }
        }

    private:
        void setup_camera(camera_control& control) {
            camera& main_camera = context<camera>();            
            const world_grid& grid = context<world_grid>();

            const point min_bounds{};
            const point max_bounds = {
                .x = min_bounds.x + (grid.width() * grid.cell_size()),
                .y = min_bounds.y + (grid.height() * grid.cell_size()),
            };

            control.min_bounds = min_bounds;
            control.max_bounds = max_bounds;

            main_camera.set_position({
                .x = (min_bounds.x + max_bounds.x) / 2.0f,
                .y = (min_bounds.y + max_bounds.y) / 2.0f,
            });
        }

        point clamp_bounds(camera_control& control, point position) {
            const point min_bounds = control.min_bounds;
            const point max_bounds = control.max_bounds;

            return {
                .x = std::clamp(position.x, min_bounds.x, max_bounds.x),
                .y = std::clamp(position.y, min_bounds.y, max_bounds.y)
            };
        }

        void drag_camera(camera_control& control) {
            const point mouse_posiiton = input::mouse_position();

            if (input::mouse_button_pressed(input::mouse::left)) {
                control.last_mouse_position = mouse_posiiton;
            }

            if (input::mouse_button_held(input::mouse::left)) {
                camera& main_camera = context<camera>();     

                const point delta = mouse_posiiton - control.last_mouse_position;
                const point position = main_camera.get_position();
                const float zoom = main_camera.get_zoom();
                const point drag_offset = {
                    .x = position.x - delta.x / zoom,
                    .y = position.y - delta.y / zoom,
                };

                main_camera.set_position(clamp_bounds(control, drag_offset));

                control.last_mouse_position = mouse_posiiton;
            }
        }

        void zoom_camera(const camera_control& control) {
            camera& main_camera = context<camera>();     

            const float scroll = input::mouse_scroll_delta();
            const float prev_zoom = main_camera.get_zoom();

            if (scroll == 0.0f) return;

            const float zoom = std::clamp(
                prev_zoom + scroll * control.zoom_increments,
                control.min_zoom,
                control.max_zoom
            );

            main_camera.set_zoom(zoom);
        }
    };
}