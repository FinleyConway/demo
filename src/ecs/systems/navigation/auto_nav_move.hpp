#pragma once

#include "ecs/systems/system_base.hpp"

#include "ecs/components/navigation.hpp"
#include "ecs/components/positional.hpp"

#include "grid/world_grid.hpp"

namespace demo::ecs {
    class auto_nav_move : public system_base {
    public:
        using system_base::system_base;    

        void update(float dt) override {
            auto view = registry().view<
                world_position,
                navigation_path,
                navigation_motion
            >();

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

            grid.at(previous_cell).set_entity(entt::null);
            grid.at(next_cell).set_entity(e);

            pos.set(next_world_pos, registry(), e);

            motion.start_position = next_world_pos;
            motion.elapsed = 0.0f;

            path.cells.pop_back();
        }
    };
}