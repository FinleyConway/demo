#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/camera/components/camera_zoom.hpp"

#include "core/input/input.hpp"
#include "rendering/camera.hpp"

namespace demo::ecs {
    class camera_zooming : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<
                camera_zoom
            >();

            for (auto [e, zoom] : view.each()) {
                zoom_camera(zoom);
            }
        }

    private:
        void zoom_camera(const camera_zoom& camera_zoom) {
            camera& main_camera = context<camera>();     

            const float scroll = input::mouse_scroll_delta();
            const float prev_zoom = main_camera.get_zoom();

            if (scroll == 0.0f) return;

            const float zoom = std::clamp(
                prev_zoom + scroll * camera_zoom.increments,
                camera_zoom.min,
                camera_zoom.max
            );

            main_camera.set_zoom(zoom);
        }
    };
}