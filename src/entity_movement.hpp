#pragma once

#include "point.hpp"

#include "grid/path_finder.hpp"

namespace demo {
    class entity_movement {
    public:
        explicit entity_movement(path_finder& path_finder) :
            m_path_finder(path_finder) 
        {
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

            m_path_request = m_path_finder.request(cell_position(), target);
        }

        void set_world_position(const point& p) {
            m_world_pos = p;
        }

        cell cell_position() const {
            return grid<>::point_to_cell(m_world_pos, c_cell_size);
        }

        const point& world_position() const {
            return m_world_pos;
        }

    private:
        void move_to_cell(float dt) {
            // no path
            if (m_current_path.empty()) return;

            cell next_cell = m_current_path.back(); // path is in reverse
            point next_world_pos = grid<>::cell_to_point(next_cell, c_cell_size);

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
        path_finder& m_path_finder;
        path_request m_path_request;
        std::vector<cell> m_current_path;

        point m_world_pos;
        point m_start_world_pos;
        float m_time_elapsed = 0.0f;
        float m_time_to_cell = 0.5f;

        const float c_cell_size = 40.0f; // temp
    };  
}