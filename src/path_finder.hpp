#pragma once

#include <mutex>
#include <vector>
#include <cstdint>
#include <utility>
#include <unordered_map>

#include "cell.hpp"
#include "worker_pool.hpp"
#include "grid_pathing.hpp"

namespace demo {
    using path_request_id = uint64_t;

    class path_finder {
    private:
        struct path_request {
            path_request_id request_id = 0;
            cell start;
            cell end;
        };

    public:
        explicit path_finder(const grid<int, 10UL, 10UL>& world) :
            m_world(world) 
        {
        }

    public:
        path_request_id request(const cell& start, const cell& end) {
            // try to start a path
            //? maybe add a lock here if multiple threads access request
            const path_request_id id = m_next_id++; 

            m_pool.add_task(
                [this, start, end]() {
                    return grid_pathing<10, 10>::search(
                        m_world, start, end
                    );
                },
                [this, id](std::vector<cell>&& path) {
                    on_search_finish(id, std::move(path));
                }
            );

            // return reference id to poll completed tasks
            return id;
        }        

        std::vector<cell> try_get_result(path_request_id id) {
            std::lock_guard lock(m_mutex);

            // find and return path if path has been completed
            auto it = m_completed.find(id);
            if (it == m_completed.end()) return {};

            std::vector<cell> path = std::move(it->second);

            m_completed.erase(it);

            return path;
        }

    private:
        void on_search_finish(path_request_id id, std::vector<cell>&& path) {
            std::lock_guard lock(m_mutex);
                    
            m_completed.emplace(
                id,
                std::move(path)
            );
        }

    private:
        const grid<int, 10UL, 10UL>& m_world;

        path_request_id m_next_id = 0;

        std::mutex m_mutex;
        std::unordered_map<path_request_id, std::vector<cell>> m_completed;
        
        worker_pool m_pool;
    };
}