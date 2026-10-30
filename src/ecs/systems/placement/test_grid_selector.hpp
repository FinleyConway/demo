#pragma once

#include "ecs/core/system_base.hpp"
#include "ecs/prefabs/enemies/zombie.hpp"
#include "ecs/components/positional.hpp"
#include "ecs/components/navigation.hpp"

#include "core/input/input.hpp"
#include "rendering/camera.hpp"
#include "resources/asset_context.hpp"
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
                auto zombie = zombie::create(registry(), context<asset_context>());

                auto& pos = registry().get<world_position>(zombie);
                pos.set(grid.cell_to_point(grid_pos), registry(), zombie);

                registry().emplace<navigation_target>(zombie, navigation_target {
                    .value = { 14, 15 }
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