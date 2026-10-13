#include <iostream>
#include <raylib.h>

#include "grid/grid.hpp"
#include "grid/path_finder.hpp"

#include "entity_movement.hpp"

int main() {
    constexpr int cols = 10;
    constexpr int rows = 10;
    constexpr int cell_size = 40;
    constexpr int offset_x = 0;
    constexpr int offset_y = 0;

    demo::grid<int, cols, rows> grid;
    demo::path_finder path_finding(grid);
    demo::entity_movement movement(path_finding);

    movement.set_world_position({
        static_cast<float>(offset_x),
        static_cast<float>(offset_y)
    });

    InitWindow(800, 450, "Grid Pathfinding");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        bool mouse_inside =
            mouse.x >= offset_x &&
            mouse.x < offset_x + cols * cell_size &&
            mouse.y >= offset_y &&
            mouse.y < offset_y + rows * cell_size;

        int mouse_x = -1;
        int mouse_y = -1;

        if (mouse_inside) {
            mouse_x = static_cast<int>(
                (mouse.x - offset_x) / cell_size
            );
            mouse_y = static_cast<int>(
                (mouse.y - offset_y) / cell_size
            );
        }

        if (mouse_inside && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            grid.at({mouse_x, mouse_y}) = 1;
        }

        if (mouse_inside && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            movement.set_target_cell({mouse_x, mouse_y});
        }

        movement.update(GetFrameTime());

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw grid lines.
        for (int x = 0; x <= cols; ++x) {
            DrawLine(
                offset_x + x * cell_size,
                offset_y,
                offset_x + x * cell_size,
                offset_y + rows * cell_size,
                DARKGRAY
            );
        }

        for (int y = 0; y <= rows; ++y) {
            DrawLine(
                offset_x,
                offset_y + y * cell_size,
                offset_x + cols * cell_size,
                offset_y + y * cell_size,
                DARKGRAY
            );
        }

        // Start cell.
        DrawRectangle(
            offset_x + 1,
            offset_y + 1,
            cell_size - 2,
            cell_size - 2,
            GREEN
        );

        // Destination cell.
        if (mouse_inside) {
            DrawRectangle(
                offset_x + mouse_x * cell_size + 1,
                offset_y + mouse_y * cell_size + 1,
                cell_size - 2,
                cell_size - 2,
                ORANGE
            );
        }

        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                int value = grid.at({x, y});

                if (value > 0) {
                    DrawText(
                        TextFormat("%d", value),
                        offset_x + x * cell_size + 14,
                        offset_y + y * cell_size + 10,
                        20,
                        BLACK
                    );
                }
            }
        }

        const demo::point& p = movement.world_position();

        DrawRectangleV(
            { p.x, p.y },
            { cell_size - 2, cell_size - 2 },
            RED
        );

        EndDrawing();
    }

    CloseWindow();
    return 0;
}