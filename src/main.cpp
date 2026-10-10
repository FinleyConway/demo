#include <iostream>

#include "grid.hpp"
#include "grid_pathing.hpp"

struct tile {};

int main() {
    demo::grid<int, 10, 10> grid;
    auto p = demo::grid_pathing<10, 10>::search(grid, demo::cell{0, 0}, demo::cell{9, 9});

    for (const auto& cell : p) {
        std::cout << cell.x << ", " << cell.y << "\n";
    }
}