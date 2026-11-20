#include "pch.hpp"
#include "world/path_finding/path_finder.hpp"

#include "world/path_finding/grid_pathing.hpp"

namespace demo {
    path_finder::path_finder(const world_grid& world_grid) :
        c_world_grid(world_grid)
    {
    }

    path_request path_finder::request(const cell& start, const cell& end, const movement_cost& cost) {
        const path_request id { m_next_id++ };

        // add grid search to the worker pool
        m_pool.add_task(
            [this, start, end, cost]() {                 
                return grid_pathing::search({ c_world_grid, start, end, cost });
            },
            [this, id](std::vector<cell>&& path) {
                on_search_finish(id, std::move(path));
            }
        );

        // return reference id to poll completed tasks
        return id;
    }        

    std::optional<std::vector<cell>> path_finder::try_get_result(path_request request) {
        std::lock_guard lock(m_mutex); // this may be slow during a hot path

        // get and return a protential path
        auto node = m_completed.extract(request); 
        if (node.empty()) return std::nullopt;

        return std::move(node.mapped());           
    }

    void path_finder::on_search_finish(path_request request, std::vector<cell>&& path) {
        std::lock_guard lock(m_mutex);
                
        m_completed.emplace(
            request,
            std::move(path)
        );
    }
}