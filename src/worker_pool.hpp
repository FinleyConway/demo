#pragma once

#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <cstddef>
#include <utility>
#include <functional>
#include <condition_variable>

namespace demo {
    class worker_pool {
    public:
        explicit worker_pool(size_t workers = std::thread::hardware_concurrency()) {
            // cant allow 0 workers
            if (workers == 0) {
                workers = 1;
            }

            m_workers.reserve(workers);

            // start workers looking for work
            for (size_t i = 0; i < workers; i++) {
                m_workers.emplace_back([this]() {
                    worker_loop();
                });
            }
        }

        worker_pool(const worker_pool&) = delete;
        worker_pool& operator=(const worker_pool&) = delete;

        ~worker_pool() {
            {
                std::lock_guard lock(m_mutex);
                m_stopping = true;
            }

            m_condition.notify_all();

            for (auto& worker : m_workers) {
                if (worker.joinable()) {
                    worker.join();
                }
            }
        }

    public: 
        template<typename Task, typename OnFinish>
        bool add_task(Task&& task, OnFinish&& on_finish) {
            {
                std::lock_guard lock(m_mutex);

                if (m_stopping) return false;

                // add work to queue
                m_tasks.emplace(
                    [task = std::forward<Task>(task), on_finish = std::forward<OnFinish>(on_finish)]() {
                        auto result = task();
                        on_finish(std::move(result));
                    }
                );
            }

            // notify a worker waiting that a task has been provided 
            m_condition.notify_one();

            return true;
        }

    private:
        void worker_loop() {
            while (true) {
                std::function<void()> task;

                {
                    std::unique_lock lock(m_mutex);

                    // wait for a new task
                    m_condition.wait(lock, [this]() {
                        return m_stopping || !m_tasks.empty();
                    });

                    if (m_stopping && m_tasks.empty()) return;

                    // pop task off queue and process task
                    task = std::move(m_tasks.front());
                    m_tasks.pop();
                }

                task();
            }
        }

    private:
        std::mutex m_mutex;
        std::condition_variable m_condition;

        std::queue<std::function<void()>> m_tasks;
        std::vector<std::thread> m_workers;

        bool m_stopping = false;
    };
}