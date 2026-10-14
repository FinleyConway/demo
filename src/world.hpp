#pragma once

#include "world_grid.hpp"

#include "grid/grid.hpp"

#include "components/entity_movement.hpp"
#include "components/path_finder.hpp"

namespace demo {
    class world {
    public:
        world() : 
            m_finder(m_grid),
            m_movement(m_grid, m_finder) 
        {
        }
    
    public:
        void update(float dt) {
            Vector2 mouse = GetMousePosition();

            bool mouse_inside =
                mouse.x < m_grid.width()  * m_grid.cell_size() &&
                mouse.y < m_grid.height() * m_grid.cell_size();

            size_t mouse_x = -1;
            size_t mouse_y = -1;

            if (mouse_inside) {
                mouse_x = static_cast<size_t>(
                    mouse.x / m_grid.cell_size()
                );
                mouse_y = static_cast<size_t>(
                    mouse.y / m_grid.cell_size()
                );
            }

            if (mouse_inside && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                m_grid.at({ mouse_x, mouse_y }).walkable = false;
            }

            if (mouse_inside && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                m_movement.set_target_cell({ mouse_x, mouse_y });
            }

            m_movement.update(GetFrameTime());
        }

        void draw() {
            // Draw grid lines.
            for (int x = 0; x <= m_grid.height(); ++x) {
                DrawLine(
                    x * m_grid.cell_size(),
                    0,
                    x * m_grid.cell_size(),
                    m_grid.width() * m_grid.cell_size(),
                    DARKGRAY
                );
            }

            for (int y = 0; y <= m_grid.width(); ++y) {
                DrawLine(
                    0,
                    y * m_grid.cell_size(),
                    m_grid.height() * m_grid.cell_size(),
                    y * m_grid.cell_size(),
                    DARKGRAY
                );
            }

            for (size_t y = 0; y < m_grid.width(); ++y) {
                for (size_t x = 0; x < m_grid.height(); ++x) {
                    const tile& tile = m_grid.get({ x, y });

                    if (!tile.walkable) {
                        DrawRectangle(
                            x * m_grid.cell_size(), y * m_grid.cell_size(),
                            m_grid.cell_size(), m_grid.cell_size(),
                            BLACK
                        );
                    }
                }
            }

            const demo::point& p = m_movement.world_position();

            DrawRectangleV(
                { p.x, p.y },
                { m_grid.cell_size(), m_grid.cell_size() },
                RED
            );
        }

    private:
        world_grid m_grid;

        path_finder m_finder;
        entity_movement m_movement;
    };
}