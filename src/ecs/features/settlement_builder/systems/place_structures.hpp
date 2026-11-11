#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/settlement_builder/components/structures.hpp"
#include "ecs/features/settlement_builder/components/structure_size.hpp"
#include "ecs/features/settlement_builder/components/settlement_zone.hpp"

#include "core/input/input.hpp"
#include "rendering/camera.hpp"
#include "ecs/core/prefab_manager.hpp"
#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    class place_structures : public system_base {
    public:
        using system_base::system_base;

    void update(float dt) override {
        auto view = registry().view<structures, settlement_zone>();

        for (auto [e, placer, zone] : view.each()) {
            if (input::mouse_button_pressed(input::mouse::left)) {
                place_structure(placer);
            }
        } 
    }

    private:
        void place_structure(structures& structures) {
            auto& main_camera = context<camera>();
            auto& grid = context<world_grid>();
            const auto& prefabs = context<prefab_manager>();

            const auto* size = prefabs.peek<structure_size>(structures.turret_prefab);

            ASSERT(size != nullptr, "Structure prefab does not have grid_size component");

            const cell cell_pos = mouse_to_cell(main_camera, grid);

            // check if structure can be placed on grid
            if (!can_place(grid, cell_pos, size->value)) {
                LOG_INFO("Can't place entity here!");

                return;
            }; 

            auto entity = prefabs.instantiate(structures.turret_prefab);

            // place instantied entity on grid
            grid.for_each_cell(cell_pos, size->value, [&](cell current) {
                grid.at(current).set_structure(entity);
                grid.at(current).set_navigation_type(navigation_type::wall);
            });

            // set entity position based on grid position
            registry().emplace_or_replace<set_position>(entity, grid.cell_to_point(cell_pos));
        }

        bool can_place(const world_grid& grid, const cell& position, const cell& size) {
            bool valid = true;

            grid.for_each_cell(position, size, [&](cell current) {
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