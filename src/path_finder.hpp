#pragma once

#include <mutex>
#include <queue>

#include "cell.hpp"
#include "worker_pool.hpp"
#include "grid_pathing.hpp"

namespace demo {
    struct path_request {
        size_t request_id = 0;
        cell start;
        cell end;
    };

    struct path_result {
        size_t request_id = 0;
        std::vector<cell> path;
    };

    class path_finder {
    public:
        uint64_t request(const demo::grid<int, 10UL, 10UL>& world, const cell& current, const cell& end) {
            const uint64_t id = m_next_id++;

            const bool task_created = m_pool.add_task(
                [world, current, end]() {
                    return grid_pathing<10, 10>::search(world, current, end);
                },
                [this, id](std::vector<cell>&& path) {
                    on_search_finish({ id, std::move(path) });
                }
            );

            if (!task_created) {
                m_requests.emplace(current, end);
            }

            return id;
        }        

    private:
        void on_search_finish(const path_result& result) {
            std::lock_guard lock(m_mutex);
                    
            m_results.emplace(
                result.request_id,
                std::move(result.path)
            );

            if (m_requests.empty()) return;


        }

    private:
        uint64_t m_next_id = 1;

        std::mutex m_mutex;
        std::queue<path_request> m_requests;
        std::unordered_map<uint64_t, path_result> m_results;
        
        worker_pool<std::vector<cell>> m_pool;
    };
}