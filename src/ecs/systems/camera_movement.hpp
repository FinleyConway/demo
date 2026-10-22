#pragma once

#include <entt/entt.hpp>

#include "ecs/components/camera_control.hpp"
#include "utils/camera.hpp"
#include "utils/input.hpp"

namespace demo::ecs {
    class camera_movement {
    public:
        camera_movement(entt::registry& registry, camera& main_camera) :
            m_registry(registry),
            m_main_camera(main_camera)
        {
        }

    public:
        void update(float dt) {
            auto view = m_registry.view<camera_control>();

            for (auto [e, control] : view.each()) {
                const point mouse = input::mouse_position();

                if (input::mouse_button_pressed(input::mouse::left)) {
                    control.last_mouse_position = mouse;
                }

                if (input::mouse_button_held(input::mouse::left)) {
                    const point delta = mouse - control.last_mouse_position;
                    const point position = m_main_camera.get_position();
                    const point drag_offset = {
                        .x = position.x - delta.x / m_main_camera.get_zoom(),
                        .y = position.y - delta.y / m_main_camera.get_zoom(),
                    };

                    m_main_camera.set_position(drag_offset);

                    control.last_mouse_position = mouse;
                }

                const float scroll = input::mouse_scroll_delta();

                if (scroll == 0.0f) continue;

                const float zoom = std::clamp(
                    m_main_camera.get_zoom() + scroll * control.zoom_increments,
                    control.min_zoom,
                    control.max_zoom
                );

                m_main_camera.set_zoom(zoom);
            }
        }

    private:
        entt::registry& m_registry;
        camera& m_main_camera;
    };
}