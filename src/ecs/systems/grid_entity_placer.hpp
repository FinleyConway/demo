#pragma once

#include "ecs/systems/system_base.hpp"

#include "ecs/entity/test_turret.hpp"

#include "ecs/components/grid_size.hpp"

#include "grid/world_grid.hpp"
#include "utils/camera.hpp"
#include "utils/input.hpp"
#include "utils/debugging.hpp"

namespace demo::ecs {
    class grid_entity_placer : public system_base {
    public:
        using system_base::system_base;

    void update(float dt) {
        if (!input::mouse_button_pressed(input::mouse::left)) return;

        camera& main_camera = context<camera>();
        world_grid& grid = context<world_grid>();

        const cell cell_pos = mouse_to_cell(main_camera, grid);

        // need to store like a grid entity context before creating the actual entity
        if (!can_place(grid, cell_pos, cell(2, 2))) {
            LOG_INFO("Can't place entity here!");

            return;
        }; 

        auto entity = test_turret::create(registry());
        const cell& size = registry().get<grid_size>(entity).value;

        if (!can_place(grid, cell_pos, size)) {
            LOG_INFO("Can't place entity here!");
            
            return;
        };

        for_each_cell(cell_pos, size, [&](cell current) {
            grid.at(current).set_entity(entity);
        });

        auto& position = registry().get<world_position>(entity);
        position.set(grid.cell_to_point(cell_pos), registry(), entity);
    }

    private:
        // this could be a helper method inside grid?
        template<typename Fn>
        void for_each_cell(const cell& offset, const cell& size, Fn&& fn) {
            for (size_t x = 0; x < size.x; x++) {
                for (size_t y = 0; y < size.y; y++) {
                    fn(cell(x + offset.x, y + offset.y));
                }
            }
        }

        bool can_place(const world_grid& grid, const cell& position, const cell& size) {
            bool valid = true;

            for_each_cell(position, size, [&](cell current) {
                if (!grid.in_bounds(current) || !grid.get(current).empty()) {
                    valid = false;
                }
            });

            return valid;
        }

        cell mouse_to_cell(const camera& main_camera, const world_grid& grid) const {
            const point world_position = main_camera.screen_to_world(
                input::mouse_position()
            );

            return grid.point_to_cell(world_position);
        }
    };
}