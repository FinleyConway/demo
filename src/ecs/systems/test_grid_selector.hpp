#pragma once

#include <entt/entt.hpp>

#include "utils/input.hpp"
#include "utils/camera.hpp"
#include "grid/world_grid.hpp"

#include "ecs/components/navigation.hpp"
#include "ecs/systems/auto_nav_requester.hpp"

namespace demo::ecs {
    class test_grid_selector {
    public:
        test_grid_selector(entt::registry& registry, const world_grid& grid, const camera& main_camera) :
            m_registry(registry),
            c_grid(grid),
            c_main_camera(main_camera)
        {
        }

        void update(float dt) {
            auto view = m_registry.view<auto_navigation>();
            
            for (auto [e] : view.each()) {
                const cell grid_pos = mouse_to_cell();                

                // mouse not in grid within this frame
                if (!c_grid.in_bounds(grid_pos)) return;

                // send a navigation request on mouse click
                if (input::mouse_button_pressed(input::mouse::left)) {
                    // maybe this should be nav target component rather then system
                    auto_nav_requester::set_target(m_registry, e, grid_pos);
                }
            }
        }

        cell mouse_to_cell() const {
            const point world_position = c_main_camera.screen_to_world(
                input::mouse_position()
            );

            return c_grid.point_to_cell(world_position);
        }

    private:
        entt::registry& m_registry;
        const world_grid& c_grid;
        const camera& c_main_camera;
    };
}