#include <iostream>

#include "grid.hpp"
#include "path_finder.hpp"

int main() {
    demo::grid<int, 10, 10> grid;
    demo::path_finder path_finding(grid);

    demo::path_request_id id = path_finding.request(demo::cell{0, 0}, demo::cell{9, 9});

    while (true) {
        path_finding.update();

        auto path = path_finding.try_get_result(id);

        for (const auto& cell : path) {
            std::cout << cell.x << ", " << cell.y << "\n";
        }
    }
}