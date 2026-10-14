#pragma once

#include "point.hpp"

#include "components/path_finder.hpp"

namespace demo {
    class auto_navigator {
    public:
        auto_navigator(world_grid& grid, path_finder& path_finder) :
            m_world_grid(grid), 
            m_path_finder(path_finder) 
        {
            grid.subscribe_grid_change([this](const cell& cell) {
                on_grid_change(cell);
            });
        } 

    public:
        void update(float dt) {
            if (!m_path_request.path_found()) {
                m_current_path = std::move(m_path_finder.try_get_result(m_path_request));
            }

            move_to_cell(dt);
        }

    public:
        void set_target_cell(const cell& target) {
            m_time_elapsed = 0.0f;
            m_start_world_pos = m_world_pos;
            m_current_path.clear();
            m_cell_target = target;

            m_path_request = m_path_finder.request(cell_position(), target);
        }

        void set_world_position(const point& p) {
            m_world_pos = p;
        }

        cell cell_position() const {
            return m_world_grid.point_to_cell(m_world_pos);
        }

        const point& world_position() const {
            return m_world_pos;
        }

    private:
        void on_grid_change(const cell& updated_cell) {
            // maybe be a bit slow if freq
            // want to get it working first
            for (const auto& cell : m_current_path) { 
                if (updated_cell == cell) {
                    // request a new path
                    set_target_cell(m_cell_target);

                    return;
                }
            }
        }
        
        void move_to_cell(float dt) {
            // no path
            if (m_current_path.empty()) return;

            cell next_cell = m_current_path.back(); // path is in reverse
            point next_world_pos = m_world_grid.cell_to_point(next_cell);

            if (m_time_elapsed < m_time_to_cell) {
                m_world_pos = m_start_world_pos.lerp_to(next_world_pos, m_time_elapsed / m_time_to_cell);
                m_time_elapsed += dt;
            }
            else {
                m_world_pos = next_world_pos;
                m_start_world_pos = next_world_pos;

                m_time_elapsed = 0.0f;
                m_current_path.pop_back();
            }
        }

    private:
        const world_grid& m_world_grid;

        path_finder& m_path_finder;
        path_request m_path_request;
        std::vector<cell> m_current_path;

        cell m_cell_target;
        point m_world_pos;
        point m_start_world_pos;
        float m_time_elapsed = 0.0f;
        float m_time_to_cell = 0.15f;
    };  
}