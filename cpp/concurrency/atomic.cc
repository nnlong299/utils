// std::atomic is lock free template class
// It ensures that read-modify-write operations are thread-safe without requiring locks

// Basic Operations:

//      Supports atomic load, store, exchange, and compare-and-swap operations

#include "utils.h"

int main() {
    MAGENTA("task::std::atomic");
    {
        const int num_threads = 5;
        const int iterations_per_thread = 1000;

        std::atomic<int> counter{0};  // Atomic counter
        auto increment = [&](int iterations) {
            for (int i = 0; i < iterations; ++i) {
                counter.fetch_add(1);  // Atomically increment the counter
            }
        };

        std::vector<std::thread> threads;

        // Launch threads to increment the counter
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back(increment, iterations_per_thread);
        }

        // Wait for all threads to finish
        for (auto& t : threads) {
            t.join();
        }

        // Display the final counter value
        std::cout << "Final counter value: " << counter << std::endl;
    }
    CYAN("task::std::atomic");

    MAGENTA("task::std::atomic_flag");
    {
        std::atomic_flag flag = ATOMIC_FLAG_INIT;

        auto thread_function = [&](int id) {
            while (flag.test_and_set(std::memory_order_acquire)) {
                // Wait until the flag is cleared
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            std::cout << "Thread " << id << " is working.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            flag.clear(std::memory_order_release);
        };

        std::thread t1(thread_function, 1);
        std::thread t2(thread_function, 2);

        t1.join();
        t2.join();

    }
    CYAN("task::std::atomic_flag");

    MAGENTA("task::CAS::Compare-and-Swap");
    {
        std::atomic<int> value{0};

        auto update_value = [&](int expected, int new_value) {
            if (value.compare_exchange_weak(expected, new_value)) {
                std::cout << "Thread updated value to " << new_value << std::endl;
            } else {
                std::cout << "Thread failed to update value. Current value: " << value.load() << std::endl;
            }
        };

        std::thread t1(update_value, 0, 10);  // Attempt to set value to 10 if it's currently 0
        std::thread t2(update_value, 0, 20);  // Attempt to set value to 20 if it's currently 0

        t1.join();
        t2.join();

        std::cout << "Final value: " << value << std::endl;
    }
    CYAN("task::std::atomic_flag");

    return 0;
}
