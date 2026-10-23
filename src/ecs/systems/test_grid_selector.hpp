#pragma once


#include "ecs/systems/system_base.hpp"

#include "ecs/components/navigation.hpp"
#include "ecs/systems/auto_nav_requester.hpp"

#include "utils/input.hpp"
#include "utils/camera.hpp"
#include "grid/world_grid.hpp"

namespace demo::ecs {
    class test_grid_selector : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) {
            auto view = registry().view<auto_navigation>();
            
            for (auto [e] : view.each()) {
                const camera& main_camera = context<camera>();
                const world_grid& grid = context<world_grid>();

                const cell grid_pos = mouse_to_cell(main_camera, grid);                

                // mouse not in grid within this frame
                if (!grid.in_bounds(grid_pos)) return;

                // send a navigation request on mouse click
                if (input::mouse_button_pressed(input::mouse::middle)) {
                    // maybe this should be nav target component rather then system
                    auto_nav_requester::set_target(registry(), e, grid_pos);
                }
            }
        }

        cell mouse_to_cell(const camera& main_camera, const world_grid& grid) const {
            const point world_position = main_camera.screen_to_world(
                input::mouse_position()
            );

            return grid.point_to_cell(world_position);
        }
    };
}