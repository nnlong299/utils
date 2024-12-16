#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <future>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:
    ThreadPool(size_t numThreads) : stop(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex);
                        this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });

                        if (this->stop && this->tasks.empty())
                            return;

                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    // Add a new task to the pool
    template <typename Func, typename... Args>
    auto enqueue(Func&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using ReturnType = decltype(f(args...));

        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<Func>(f), std::forward<Args>(args)...)
        );

        std::future<ReturnType> res = task->get_future();

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (stop)
                throw std::runtime_error("ThreadPool has stopped!");

            tasks.emplace([task]() { (*task)(); });
        }

        condition.notify_one();
        return res;
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }

        condition.notify_all();
        for (std::thread& worker : workers) {
            if (worker.joinable())
                worker.join();
        }
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> stop;
};

int main() {
    ThreadPool pool(4);

    // Enqueue tasks
    std::vector<std::future<int>> results;
    for (int i = 0; i < 8; ++i) {
        results.emplace_back(pool.enqueue([i] {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Task " << i << " completed.\n";
            return i * i;
        }));
    }

    // Retrieve results
    for (auto& result : results) {
        std::cout << "Result: " << result.get() << "\n";
    }

    return 0;
}

// Explanation:
// Thread Pool Class:

// Worker Threads: A fixed number of threads (workers) are started when the thread pool is created.
// Task Queue: A std::queue stores tasks (std::function<void()>) to be executed by the threads.
// Condition Variable: Ensures threads only wake up when there's work to do.
// Enqueuing Tasks:

// Tasks are wrapped in a std::packaged_task to enable returning results via std::future.
// Thread Execution:

// Threads continuously fetch and execute tasks from the queue until the pool is stopped.
// Graceful Shutdown:

// When the pool is destroyed, it stops accepting tasks and joins all threads.