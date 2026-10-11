#include <iostream>

#include "grid.hpp"
#include "path_finder.hpp"

int main() {
    demo::grid<int, 10, 10> grid;
    demo::path_finder path_finding(grid);

    demo::path_request_id id = path_finding.request(demo::cell{0, 0}, demo::cell{9, 9});

    while (true) {
        path_finding.update();

        auto opt_path = path_finding.try_get_result(id);

        if (opt_path.has_value()) {
            for (const auto& cell : *opt_path) {
                std::cout << cell.x << ", " << cell.y << "\n";
            }
        }
    }
}