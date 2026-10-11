#include <iostream>

#include "grid.hpp"

#include "path_finder.hpp"

struct tile {};

void do_work() {
    for (size_t i = 0; i < 10; i++) {
        std::cout << "hello" << i << std::endl;;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void on_finish() {
    std::cout << "work finished" << std::endl;;
}

int main() {
    // demo::grid<int, 10, 10> grid;
    // auto p = demo::grid_pathing<10, 10>::search(grid, demo::cell{0, 0}, demo::cell{9, 9});

    // for (const auto& cell : p) {
    //     std::cout << cell.x << ", " << cell.y << "\n";
    // }

    demo::path_finder path_finding;
}