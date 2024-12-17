#include "utils.h"

std::mutex mtx, mtx1, mtx2;
std::condition_variable cv;
bool ready = false;
std::shared_mutex sharedMtx;
std::timed_mutex tmtx;
std::recursive_mutex recMtx;

void recursiveFunction(int count)
{
    if (count <= 0)
        return;

    recMtx.lock();
    std::cout << "Lock acquired, count: " << count << "\n";
    recursiveFunction(count - 1);
    recMtx.unlock();
}

auto main() -> int
{
    MAGENTA("task::std::lock_guard");
    {
        // 1. std::lock_guard
        // Header: <mutex>
        // Purpose: Simplest RAII-style lock that automatically acquires a lock in its constructor and releases it in its destructor.
        // Usage: Use when you need a simple, scoped-based lock without the need to unlock or relock manually.

        // When to Use:
        //      When you need to lock a single mutex for the duration of a scope.
        //      When you don't need advanced locking operations like unlocking or relocking.

        auto task = []()
        {
            std::lock_guard<std::mutex> lock(mtx); // Lock is acquired here
            std::cout << "Thread " << std::this_thread::get_id() << " is working.\n";
        }; // Lock is released automatically at the end of the scope

        std::thread t1(task);
        std::thread t2(task);

        t1.join();
        t2.join();
    }
    CYAN("task::std::lock_guard");

    MAGENTA("task::std::scoped_lock");
    {
        // 2. std::scoped_lock (C++17)
        // Header: <mutex>
        // Purpose: A modern alternative to std::lock_guard. It can lock multiple mutexes simultaneously in a deadlock-free manner.
        // Usage: Use when you need to lock one or more mutexes at once.
        //
        // When to Use:
        //      When you need to lock multiple mutexes.
        //      When using C++17 or later.

        // deadlock when using std::lock_guard
        {
            // auto thread1 = []() {
            //     std::lock_guard<std::mutex> lock1(mtx1); // T1 locks mtx1
            //     std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work
            //     std::lock_guard<std::mutex> lock2(mtx2); // T1 tries to lock mtx2
            //     std::cout << "Thread 1 acquired both locks\n";
            // };

            // auto thread2 = []() {
            //     std::lock_guard<std::mutex> lock2(mtx2); // T2 locks mtx2
            //     std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work
            //     std::lock_guard<std::mutex> lock1(mtx1); // T2 tries to lock mtx1
            //     std::cout << "Thread 2 acquired both locks\n";
            // };

            // std::thread t1(thread1);
            // std::thread t2(thread2);

            // t1.join();
            // t2.join();
        }

        // working
        {
            auto task = []()
            {
                // std::scoped_lock locks multiple mutexes atomically (at the same time) in a deadlock-free manner.
                std::scoped_lock lock(mtx1, mtx2);
                std::cout << "Thread " << std::this_thread::get_id() << " acquired locks.\n";
            };

            std::thread t1(task);
            std::thread t2(task);

            t1.join();
            t2.join();
        }
    }
    CYAN("task::std::scoped_lock");

    MAGENTA("task::std::unique_lock");
    {
        // 3. std::unique_lock
        // Header: <mutex>
        // Purpose: A flexible RAII-style lock that supports advanced features like:
        //      Manual unlocking/relocking: This can be useful in scenarios where you need to temporarily release a lock to perform an operation that doesn't require the mutex, and then relock it afterward.
        //      Deferred locking (lock later): leave the lock to control it manually later, is used with below point.
        //      Timed locking and try-lock operations.
        // Usage: Use when you need more control over the locking behavior.
        // When to Use:
        //      When you need to unlock and relock the mutex during the scope (e.g., for std::condition_variable::wait).
        //      When you want to lock the mutex later (deferred locking).
        //      When you need to use timed or try-lock mechanisms.

        { // Manual unlocking/relocking:
            auto worker = []()
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, []
                        { return ready; }); // Temporarily unlocks the mutex
                std::cout << "Thread " << std::this_thread::get_id() << " is working.\n";
            };

            std::thread t(worker);

            std::this_thread::sleep_for(std::chrono::seconds(1));
            {
                std::lock_guard<std::mutex> lock(mtx);
                ready = true;
            }
            cv.notify_one();

            t.join();
        }

        { // Deferred locking + Timed locking and try-lock -> [try_lock_for]
            auto worker = [](int id)
            {
                std::unique_lock<std::mutex> lock(mtx, std::defer_lock); // Defer lock
                std::cout << "Worker " << id << " attempting to acquire lock for 1 second.\n";

                if (lock.try_lock_for(std::chrono::seconds(1)))
                {
                    std::cout << "Worker " << id << " acquired the lock.\n";
                    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate work
                }
                else
                {
                    std::cout << "Worker " << id << " failed to acquire the lock in time.\n";
                }
            };

            std::thread t1(worker, 1);
            std::thread t2(worker, 2);

            t1.join();
            t2.join();
        }

        { // Deferred locking + Timed locking and try-lock -> [try_lock_until]
            auto worker = [](int id)
            {
                std::unique_lock<std::mutex> lock(mtx, std::defer_lock); // Defer lock
                auto timeout = std::chrono::system_clock::now() + std::chrono::seconds(1);
                std::cout << "Worker " << id << " attempting to acquire lock until timeout.\n";

                if (lock.try_lock_until(timeout))
                {
                    std::cout << "Worker " << id << " acquired the lock.\n";
                    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate work
                }
                else
                {
                    std::cout << "Worker " << id << " failed to acquire the lock in time.\n";
                }
            };

            std::thread t1(worker, 1);
            std::thread t2(worker, 2);

            t1.join();
            t2.join();
        }

        { // Deferred locking + Timed locking and try-lock -> [try_lock]
            auto worker = [](int id) {
                std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
                
                // try_lock only once
                if (lock.try_lock()) {
                    std::cout << "Worker " << id << " acquired the lock.\n";
                    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate work
                } else {
                    std::cout << "Worker " << id << " could not acquire the lock.\n";
                }
            };

            std::thread t1(worker, 1);
            std::thread t2(worker, 2);

            t1.join();
            t2.join();
        }
    }
    CYAN("task::std::unique_lock");

    MAGENTA("task::std::shared_lock");
    {
        // 4. std::shared_lock (C++17)
        // Header: <shared_mutex>
        // Purpose: A lock for shared ownership. Multiple readers can acquire the lock simultaneously, but a writer has exclusive access.
        // Usage: Use when you need read-write synchronization (readers-writers problem).
        // When to Use:
        //      When multiple threads need to read shared data simultaneously (shared lock).
        //      When only one thread should write to the shared data at a time (exclusive lock)
        auto reader = [](int id)
        {
            std::shared_lock<std::shared_mutex> lock(sharedMtx); // Shared lock for reading
            std::cout << "Reader " << id << " is reading.\n";
        };

        auto writer = []()
        {
            std::unique_lock<std::shared_mutex> lock(sharedMtx); // Exclusive lock for writing
            std::cout << "Writer is writing.\n";
        };

        std::thread r1(reader, 1);
        std::thread r2(reader, 2);
        std::thread w(writer);

        r1.join();
        r2.join();
        w.join();
    }
    CYAN("task::std::shared_lock");

    MAGENTA("task::std::timed_mutex");
    {
        // 5. std::timed_mutex
        // Header: <mutex>
        // Purpose: Provides a mutex with timeout features (try_lock_for and try_lock_until).
        // Usage: Use when you need to acquire a lock with a time limit.
        // When to Use:
        //      When you want to avoid indefinite blocking by providing a timeout for lock acquisition.

        auto task = [](int id)
        {
            if (tmtx.try_lock_for(std::chrono::milliseconds(100)))
            { // Try to lock with timeout
                std::cout << "Thread " << id << " acquired the lock.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                tmtx.unlock();
            }
            else
            {
                std::cout << "Thread " << id << " could not acquire the lock.\n";
            }
        };

        std::thread t1(task, 1);
        std::thread t2(task, 2);

        t1.join();
        t2.join();
    }
    CYAN("task::std::timed_mutex");

    {
        // 6. std::recursive_mutex
        // Header: <mutex>
        // Purpose: Allows the same thread to acquire the lock multiple times recursively.
        // Usage: Use when a thread needs to acquire the same mutex multiple times (e.g., recursive functions).
        // When to Use:
        //      When you have recursive functions that require locking.

        std::thread t1(recursiveFunction, 3);
        t1.join();
    }

    // WHEN TO USE WHICH
    {
        // | **Lock Type**           | **Header**           | **Purpose**                                    | **Use Case**                        |
        // |-------------------------|----------------------|------------------------------------------------|-------------------------------------|
        // | `std::lock_guard`       | `<mutex>`            | Simple scoped-based lock                       | Single mutex, no unlock/relock      |
        // | `std::scoped_lock`      | `<mutex>` (C++17)    | Lock multiple mutexes simultaneously           | Deadlock-free multi-mutex locking   |
        // | `std::unique_lock`      | `<mutex>`            | Flexible lock with manual unlock/relock        | Condition variables, timed locking  |
        // | `std::shared_lock`      | `<shared_mutex>`     | Shared ownership lock (readers-writers problem)| Multiple readers, single writer     |
        // | `std::timed_mutex`      | `<mutex>`            | Lock with timeout options                      | Avoid indefinite blocking           |
        // | `std::recursive_mutex`  | `<mutex>`            | Allows recursive locking by the same thread    | Recursive function synchronization  |

        // Use std::lock_guard for simple scoped locks.
        // Use std::scoped_lock for locking multiple mutexes (C++17).
        // Use std::unique_lock for advanced locking needs (e.g., std::condition_variable).
        // Use std::shared_lock for read-heavy workloads with shared ownership (C++17).
        // Use std::timed_mutex if you need timeouts on lock acquisition.
        // Use std::recursive_mutex for recursive locking.
    }

    return 0;
}