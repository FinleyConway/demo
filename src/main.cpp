#include <iostream>
#include <raylib.h>

#include "grid.hpp"
#include "path_finder.hpp"

int main() {
    constexpr int cols = 10;
    constexpr int rows = 10;
    constexpr int cell_size = 40;
    constexpr int offset_x = 100;
    constexpr int offset_y = 25;

    demo::grid<int, cols, rows> grid;
    demo::path_finder path_finding(grid);
    std::vector<demo::cell> cached_path;

    InitWindow(800, 450, "Grid Pathfinding");
    SetTargetFPS(60);

    demo::path_request_id id{};
    int last_x = -1;
    int last_y = -1;
    bool request_pending = false;

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

        // Request a new path only when the destination cell changes.
        if (mouse_inside &&
            (mouse_x != last_x || mouse_y != last_y)) {
            id = path_finding.request(
                demo::cell{0, 0},
                demo::cell{mouse_x, mouse_y}
            );

            last_x = mouse_x;
            last_y = mouse_y;
            request_pending = true;
        }

        if (mouse_inside && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            grid.at(demo::cell{mouse_x, mouse_y})++;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the path returned by your pathfinder.
        if (request_pending) {
            auto path = path_finding.try_get_result(id);

            if (!path.empty()) {
                cached_path = std::move(path);
                request_pending = false;
            }
        }

        for (const auto& cell : cached_path) {
            DrawRectangle(
                offset_x + cell.x * cell_size + 1,
                offset_y + cell.y * cell_size + 1,
                cell_size - 2,
                cell_size - 2,
                SKYBLUE
            );
        }

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

        DrawText(
            "Move mouse to find a path",
            offset_x,
            offset_y + rows * cell_size + 12,
            20,
            DARKGRAY
        );

        EndDrawing();
    }

    CloseWindow();
    return 0;
}