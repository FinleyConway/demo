#include <iostream>

#include "grid.hpp"
#include "grid_pathing.hpp"

#include "worker_pool.hpp"

struct tile {};

void do_work() {
    for (size_t i = 0; i < 10; i++) {
        std::cout << "hello" << i << std::endl;;
    }
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

    demo::worker_pool pool;

    pool.add_task(do_work, on_finish);
    pool.add_task(do_work, on_finish);
    pool.add_task(do_work, on_finish);


    for (auto i = 0; i < 10; i++) {
        std::cout << "main thread\n";
    }
}