#pragma once

#include <future>
#include <chrono>
#include <thread>
#include <vector>
#include <cstddef>
#include <utility>
#include <functional>

namespace demo {
    template<typename T>
    class worker_pool {
    public:
        explicit worker_pool(size_t workers = std::thread::hardware_concurrency()) : 
            m_workers(workers) 
        {
        }
    
    public:
        template<typename Task, typename OnFinish>
        bool add_task(Task&& task, OnFinish&& on_finish) {
            for (auto& worker : m_workers) {
                if (!available_task(worker)) continue;

                worker = std::async(
                    std::launch::async,
                    [task = std::forward<Task>(task), on_finish = std::forward<OnFinish>(on_finish)]() {
                        T result = task();
                        on_finish(std::move(result));
                    }
                );

                return true;
            }

            return false;
        }

    private:
        bool available_task(std::future<void>& worker) const {
            return
                !worker.valid() ||
                worker.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
        }

    private:
        std::vector<std::future<void>> m_workers;
    };
}