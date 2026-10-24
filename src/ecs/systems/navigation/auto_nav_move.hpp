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
            // no path
            if (path.cells.empty()) return;

            const cell next_cell = path.cells.back(); // path is in reverse
            const point next_world_pos = context<world_grid>().cell_to_point(next_cell);
            point new_position;

            if (motion.elapsed < motion.time_per_cell) {
                new_position = motion.start_position.lerp_to(next_world_pos, motion.elapsed / motion.time_per_cell);
                motion.elapsed += dt;
            }
            else {
                new_position = next_world_pos;
                motion.start_position = next_world_pos;

                motion.elapsed = 0.0f;
                path.cells.pop_back();
            }

            pos.set(new_position, registry(), e);
        }
    };
}