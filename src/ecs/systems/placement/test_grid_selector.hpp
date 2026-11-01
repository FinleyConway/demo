#pragma once

#include "core/debugging.hpp"
#include "ecs/components/health.hpp"
#include "ecs/core/prefab_manager.hpp"
#include "ecs/core/system_base.hpp"
#include "ecs/components/positional.hpp"
#include "ecs/components/navigation.hpp"

#include "core/input/input.hpp"
#include "rendering/camera.hpp"
#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    class test_grid_selector : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) {
            const camera& main_camera = context<camera>();
            const world_grid& grid = context<world_grid>();

            const cell grid_pos = mouse_to_cell(main_camera, grid);    

            if (input::mouse_button_pressed(input::mouse::middle)) {
                const auto& p_mananger = context<prefab_manager>();

                const auto* h = p_mananger.peak<health>(0);

                LOG_INFO("Prefab heath: %f", h->current_health);

                auto zombie = p_mananger.instantiate(0);

                auto& pos = registry().get<world_position>(zombie);
                pos.set(grid.cell_to_point(grid_pos), registry(), zombie);

                registry().emplace<navigation_target>(zombie, navigation_target {
                    .value = { 15, 15 }
                });
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