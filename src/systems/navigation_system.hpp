#pragma once

#include <entt/entt.hpp>

#include "grid/world_grid.hpp"
#include "grid/pathing/path_finder.hpp"

#include "components/navigation.hpp"
#include "components/world_position.hpp"

namespace demo {
    class navigation_system {
    public:
        navigation_system(world_grid& grid, entt::registry& registry) :
            m_grid(grid),
            m_registry(registry),
            m_path_finder(grid)
        {
        }
    
    public:
        void update(float dt) {
            auto view = m_registry.view<
                auto_navigation,
                world_position,
                navigation_target,
                navigation_path,
                navigation_motion,
                navigation_request_state
            >();

            // handle entity repath when the grid has changed
            if (m_grid.has_changed()) {
                std::unordered_set<cell> changed_cells = m_grid.consume_changed_cells();

                for (auto entity : view) {
                    need_repath(entity, changed_cells);
                }    
            }

            for (auto entity : view) {
                update_entity(entity, dt);
            }
        }

        bool set_target(entt::entity entity, const cell& target) {
            // doesnt have required components
            if (!has_required_components(entity)) return false;

            auto& pos = m_registry.get<world_position>(entity);
            auto& target_component = m_registry.get<navigation_target>(entity);
            auto& path = m_registry.get<navigation_path>(entity);
            auto& motion = m_registry.get<navigation_motion>(entity);
            auto& request = m_registry.get<navigation_request_state>(entity);

            // set target cell and reset path
            target_component.value = target;
            path.cells.clear();

            // reset movement
            motion.elapsed = 0.0f;
            motion.start_position = pos.value;

            // request a new path
            request.request = m_path_finder.request(
                m_grid.point_to_cell(pos.value),
                target
            );

            return true;
        }

    private:
        bool has_required_components(entt::entity entity) const {
            return m_registry.valid(entity)
                && m_registry.all_of<
                    auto_navigation,
                    world_position,
                    navigation_target,
                    navigation_path,
                    navigation_motion,
                    navigation_request_state
                >(entity);
        }

        void need_repath(entt::entity entity, const std::unordered_set<cell>& changed_cells) {
            const auto& path = m_registry.get<navigation_path>(entity);

            for (const auto& c : path.cells) { 
                if (!changed_cells.contains(c)) continue; 

                // request a new path
                const cell target = m_registry.get<navigation_target>(entity).value;

                set_target(entity, target);

                return;
            }
        }

        void update_entity(entt::entity entity, float dt) {
            auto& pos = m_registry.get<world_position>(entity);
            auto& target = m_registry.get<navigation_target>(entity);
            auto& path = m_registry.get<navigation_path>(entity);
            auto& motion = m_registry.get<navigation_motion>(entity);
            auto& request = m_registry.get<navigation_request_state>(entity);

            if (!request.request.path_found()) {
                path.cells = std::move(m_path_finder.try_get_result(request.request));
            }

            move_to_cell(pos, path, motion, dt);
        }

        void move_to_cell(world_position& pos, navigation_path& path, navigation_motion& motion, float dt) {
            // no path
            if (path.cells.empty()) return;

            const cell next_cell = path.cells.back(); // path is in reverse
            const point next_world_pos = m_grid.cell_to_point(next_cell);

            if (motion.elapsed < motion.time_per_cell) {
                pos.value = motion.start_position.lerp_to(next_world_pos, motion.elapsed / motion.time_per_cell);
                motion.elapsed += dt;
            }
            else {
                pos.value = next_world_pos;
                motion.start_position = next_world_pos;

                motion.elapsed = 0.0f;
                path.cells.pop_back();
            }
        }

    private:
        world_grid& m_grid; 
        entt::registry& m_registry;

        path_finder m_path_finder;
    };
}