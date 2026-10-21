#pragma once

#include <entt/entt.hpp>

#include "grid/world_grid.hpp"

#include "ecs/components/navigation.hpp"
#include "ecs/components/positional.hpp"

namespace demo::ecs {
    class auto_nav_move {
    public:
        auto_nav_move(const world_grid& grid, entt::registry& registry) :
            c_grid(grid),
            m_registry(registry)
        {
        }         

    public:
        void update(float dt) {
            auto view = m_registry.view<
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
            const point next_world_pos = c_grid.cell_to_point(next_cell);
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

            pos.set(new_position, m_registry, e);
        }

    private:
        const world_grid& c_grid; 
        entt::registry& m_registry;
    };
}