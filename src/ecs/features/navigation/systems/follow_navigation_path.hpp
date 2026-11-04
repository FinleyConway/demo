#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/components/positional.hpp"

#include "ecs/features/navigation/components/navigation_path.hpp"
#include "ecs/features/navigation/components/navigation_motion.hpp"
#include "ecs/features/navigation/components/navigation_obstruction.hpp"

#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    class follow_navigation_path : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<
                world_position,
                navigation_path,
                navigation_motion
            >(entt::exclude<navigation_obstruction>);

            for (auto [e, world_position, nav_path, nav_mov] : view.each()) {
                move_to_cell(e, world_position, nav_path, nav_mov, dt);
            }
        }

    private:
        void move_to_cell(entt::entity e, world_position& pos, navigation_path& path, navigation_motion& motion, float dt) {
            if (path.cells.empty()) return;

            world_grid& grid = context<world_grid>();

            const cell next_cell = path.cells.back();
            const point next_world_pos = grid.cell_to_point(next_cell);

            // check if the next cell is free
            if (grid.get(next_cell).has_structure(registry())) {
                registry().emplace_or_replace<navigation_obstruction>(
                    e, 
                    navigation_obstruction {
                        .tile = next_cell
                    }
                );

                // dont want the entity to move until the obstruction is cleared
                return;
            }


            if (motion.elapsed < motion.time_per_cell) {
                const point new_position = motion.start_position.lerp_to(
                    next_world_pos,
                    motion.elapsed / motion.time_per_cell
                );

                motion.elapsed += dt;
                pos.set(new_position, registry(), e);

                return;
            }

            const cell previous_cell = grid.point_to_cell(
                motion.start_position
            );

            grid.at(previous_cell).remove_entity(e);
            grid.at(next_cell).add_entity(e);

            pos.set(next_world_pos, registry(), e);

            motion.start_position = next_world_pos;
            motion.elapsed = 0.0f;

            path.cells.pop_back();
        }
    };
}