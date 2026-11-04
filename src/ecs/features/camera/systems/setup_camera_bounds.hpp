#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/camera/components/camera_bounds.hpp"

#include "rendering/camera.hpp"
#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    class setup_camera_bounds : public system_base {
    public:
        using system_base::system_base;

        void start() override {
            auto view = registry().view<camera_bounds, camera_bounds_init>();

            for (auto [e, control] : view.each()) {
                setup_camera(control);

                registry().remove<camera_bounds_init>(e);
            }
        }

    private:
        void setup_camera(camera_bounds& bounds) {
            camera& main_camera = context<camera>();            
            const world_grid& grid = context<world_grid>();

            const point min_bounds{};
            const point max_bounds = {
                .x = min_bounds.x + (grid.width() * grid.cell_size()),
                .y = min_bounds.y + (grid.height() * grid.cell_size()),
            };

            bounds.min = min_bounds;
            bounds.max = max_bounds;

            main_camera.set_position({
                .x = (min_bounds.x + max_bounds.x) / 2.0f,
                .y = (min_bounds.y + max_bounds.y) / 2.0f,
            });
        }
    };
}