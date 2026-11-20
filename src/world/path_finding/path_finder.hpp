#pragma once

#include <mutex>
#include <vector>
#include <cstdint>
#include <unordered_map>

#include "core/threading/worker_pool.hpp"

#include "world/grid/cell.hpp"
#include "world/path_finding/movement_profile.hpp"
#include "world/world_grid/world_grid.hpp"

namespace demo {
    enum class path_request : uint32_t {};

    class path_finder {
    public:
        explicit path_finder(const world_grid& world_grid);

        path_request request(const cell& start, const cell& end, const movement_cost& cost);    

        std::optional<std::vector<cell>> try_get_result(path_request request);

    private:
        void on_search_finish(path_request request, std::vector<cell>&& path);

    private:
        const world_grid& c_world_grid;

        uint32_t m_next_id = 0;
        std::unordered_map<path_request, std::vector<cell>> m_completed;

        std::mutex m_mutex;
        worker_pool m_pool;
    };
}