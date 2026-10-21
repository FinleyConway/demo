#pragma once

#include <raylib.h>
#include <entt/entt.hpp>

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
                Vector2 mouse = GetMousePosition();
                point world_position = c_main_camera.screen_to_world({
                    .x =  mouse.x,
                    .y = mouse.y
                });

                bool mouse_inside =
                    world_position.x < c_grid.width() * c_grid.cell_size() &&
                    world_position.y < c_grid.height() * c_grid.cell_size();

                size_t mouse_x = 0;
                size_t mouse_y = 0;

                if (mouse_inside) {
                    mouse_x = static_cast<size_t>(
                        world_position.x / c_grid.cell_size()
                    );
                    mouse_y = static_cast<size_t>(
                        world_position.y / c_grid.cell_size()
                    );
                }

                if (mouse_inside && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    ecs::auto_nav_requester::set_target(m_registry, e, { mouse_x, mouse_y });
                }
            }
        }

    private:
        entt::registry& m_registry;
        const world_grid& c_grid;
        const camera& c_main_camera;
    };
}