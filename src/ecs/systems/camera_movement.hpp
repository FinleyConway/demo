#pragma once

#include <entt/entt.hpp>

#include "ecs/components/camera_control.hpp"
#include "grid/world_grid.hpp"
#include "utils/camera.hpp"
#include "utils/input.hpp"

namespace demo::ecs {
    class camera_movement {
    public:
        camera_movement(entt::registry& registry, camera& main_camera, const world_grid& grid) :
            m_registry(registry),
            m_main_camera(main_camera),
            c_grid(grid)
        {
        }

    public:
        void update(float dt) {
            auto view = m_registry.view<camera_control>();

            for (auto [e, control] : view.each()) {
                setup_camera(control);

                zoom_camera(control);
                drag_camera(control);
            }
        }

    private:
        void setup_camera(camera_control& control) {
            if (control.setup) return; // temp, should just create a start func

            const point min_bounds{};
            const point max_bounds = {
                .x = min_bounds.x + (c_grid.width() * c_grid.cell_size()),
                .y = min_bounds.y + (c_grid.height() * c_grid.cell_size()),
            };

            control.min_bounds = min_bounds;
            control.max_bounds = max_bounds;

            m_main_camera.set_position({
                .x = (min_bounds.x + max_bounds.x) / 2.0f,
                .y = (min_bounds.y + max_bounds.y) / 2.0f,
            });

            control.setup = true;
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
                const point delta = mouse_posiiton - control.last_mouse_position;
                const point position = m_main_camera.get_position();
                const float zoom = m_main_camera.get_zoom();
                const point drag_offset = {
                    .x = position.x - delta.x / zoom,
                    .y = position.y - delta.y / zoom,
                };

                m_main_camera.set_position(clamp_bounds(control, drag_offset));

                control.last_mouse_position = mouse_posiiton;
            }
        }

        void zoom_camera(const camera_control& control) {
            const float scroll = input::mouse_scroll_delta();
            const float prev_zoom = m_main_camera.get_zoom();

            if (scroll == 0.0f) return;

            const float zoom = std::clamp(
                prev_zoom + scroll * control.zoom_increments,
                control.min_zoom,
                control.max_zoom
            );

            m_main_camera.set_zoom(zoom);
        }

    private:
        entt::registry& m_registry;
        camera& m_main_camera;
        const world_grid& c_grid;
    };
}