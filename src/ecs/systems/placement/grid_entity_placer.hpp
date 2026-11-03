#pragma once

#include "ecs/core/system_base.hpp"
#include "ecs/core/prefab_manager.hpp"
#include "ecs/components/grid_data.hpp"
#include "ecs/components/positional.hpp"
#include "ecs/components/base_management.hpp"

#include "core/debugging.hpp"
#include "core/input/input.hpp"
#include "rendering/camera.hpp"
#include "world/world_grid/world_grid.hpp"
#include "world/path_finding/movement_profile.hpp"

namespace demo::ecs {
    class grid_entity_placer : public system_base {
    public:
        using system_base::system_base;

    void update(float dt) override {
        auto view = registry().view<defence_placer, base_defence_zone>();

        for (auto [e, placer, zone] : view.each()) {
            if (input::mouse_button_pressed(input::mouse::left)) {
                place_structure(placer);
            }
        } 
    }

    private:
        void place_structure(defence_placer& placer) {
            auto& main_camera = context<camera>();
            auto& grid = context<world_grid>();
            const auto& prefabs = context<prefab_manager>();

            const auto* size = prefabs.peak<grid_size>(placer.wall_prefab);

            ASSERT(size != nullptr, "Structure prefab does not have grid_size component");

            const cell cell_pos = mouse_to_cell(main_camera, grid);

            // check if structure can be placed on grid
            if (!can_place(grid, cell_pos, size->value)) {
                LOG_INFO("Can't place entity here!");

                return;
            }; 

            auto entity = prefabs.instantiate(placer.wall_prefab);

            // place instantied entity on grid
            for_each_cell(cell_pos, size->value, [&](cell current) {
                grid.at(current).set_structure(entity);
                grid.at(current).set_navigation_type(navigation_type::wall);
            });

            // set entity position based on grid position
            auto& position = registry().get<world_position>(entity);
            position.set(grid.cell_to_point(cell_pos), registry(), entity);
        }

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
                if (!grid.in_bounds(current) || grid.get(current).has_structure(registry())) {
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