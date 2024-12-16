#include "utils.h"

class Worker {
public:
    void doWork(int id) {
        std::cout << "Worker " << id << " is working.\n";
    }
};

void task1() // join -> expect the inside's log show between start-end
{
    std::cout << "Task 1 is running.\n";
}

void task2() // detach-> expect the inside's log show outside of start-end
{
    std::cout << "Task 2 is running.\n";
}

int main()
{
    MAGENTA("task1::start");
    {
        // normal function
        {
            std::thread t1(task1); // the thread is created and started immediately
            t1.join(); // caller's thread waits for t1 to finish
        }

        // lambda
        {
            // note: capture carefully, if you capture the shared data
            // do protect your data
            std::thread t1([]()
            {
                std::cout << "Lambdaaaaaaaaaaaaaaaaa\n";
            }); // the thread is created and started immediately
            t1.join(); // caller's thread waits for t1 to finish
        }

        {
            Worker worker;
            // Use a lambda to call the member function
            std::thread t([&worker]()
            {
                worker.doWork(1);
            });
            t.join(); // Wait for thread to finish
        }
    }
    CYAN("task1::end");

    MAGENTA("task2::start");
    {
        std::thread t2(task2); // the thread is created and started immediately
        t2.detach(); //  caller's thread doesnot wait for t2, t2 runs independently
    }
    RED("task2::end we suppose so but reality is not");

    MAGENTA("task3::start");
    {
        // join - thread can take parameters
        auto task3 = [](int x, int y)
        {
            std::cout << "Task 3 is running.\n";
            std::cout << "x = " << x << "\n";
            std::cout << "y = " << y << "\n";
        };

        auto task3_ref = [](int x, int& y)
        {
            std::cout << "Task 3 ref is running.\n";
            std::cout << "x = " << x << "\n";
            y = 11111;
            std::cout << "y = " << y << "\n";
        };

        // copy parameter
        {
            std::thread t3(task3, 1 , 2); // the thread is created and started immediately
            t3.join(); // caller's thread waits for t1 to finish   
        }
        // ref parameter
        {
            int y = 2;
            std::thread t3(task3_ref, 1 , std::ref(y)); // the thread is created and started immediately
            WHITE("y = ", y);
            t3.join(); // caller's thread waits for t1 to finish   
        }
    }
    CYAN("task3::end");

    MAGENTA("task4::start");
    {
        // TODO: enable this block to try case 4, it will stop the rest of program by std::terminate

        // join - thread cannot propagate std::exception to let caller handle
        // std::terminate is called immediately, leading to program termination.
        // auto task4_problematic_function = []()
        // {
        //     std::cout << "Thread started!" << std::endl;

        //     // Simulating an error
        //     throw std::runtime_error("An error occurred in the thread!");

        //     // This return value is ignored
        //     std::cout << "Thread is finishing execution." << std::endl;
        // };
        // try 
        // {
        //     // Start a thread that will throw an exception
        //     std::thread t(task4_problematic_function);

        //     // Wait for the thread to finish execution
        //     t.join();
        // } catch (const std::exception& ex) {
        //     // This block will not be executed because the exception in the thread
        //     // does not propagate to the main thread.
        //     std::cerr << "Caught exception: " << ex.what() << std::endl;
        // }
    }
    CYAN("task4::end");

    MAGENTA("task5::start"); // join - thread has to itself handle exception
    {
        auto task5_safe_function = []()
        {
            try
            {
                std::cout << "Thread started!" << std::endl;
                throw std::runtime_error("An error occurred in the thread!");
            }
            catch (const std::exception& e)
            {
                std::cerr << "Caught exception in thread: " << e.what() << std::endl;
            }
        };

        std::thread t(task5_safe_function); // the thread is created and started immediately
        t.join(); // caller's thread waits for t to finish   
    }
    CYAN("task5::end");

    // task 6
    {
        // join 
        //      sync 2 threads with std::promise
        //      std::promise set shared value and propagate exception back to caller
        //      std::future get the value of std::promise, and continue caller's thread without needing the end of worker thread
        auto task6_compute_factorial = [](int n, std::promise<int> result_promise)
        {
            try {
                if (n < 0) {
                    throw std::invalid_argument("Negative number provided!");
                }
                int result = 1;
                for (int i = 1; i <= n; ++i) {
                    result *= i;
                }
                result_promise.set_value(result); // Communicate the result
            } catch (...) {
                result_promise.set_exception(std::current_exception()); // Communicate the exception
            }

            std::cout<<"Sleep 1s after std::promise::set()\n";
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1s);
            std::cout<<"Sleep is done\n";
        };

        MAGENTA("task6::start");
        {
            // Create a promise to hold the result or exception
            std::promise<int> result_promise;
            
            // Get a future from the promise
            std::future<int> result_future = result_promise.get_future();
            
            // Start a thread and pass the promise
            std::thread t(task6_compute_factorial, 5, std::move(result_promise));
            
            try {
                // Get the result (blocks until the promise is fulfilled)
                int result = result_future.get();
                std::cout << "Factorial result: " << result << std::endl;
            } catch (const std::exception& e) {
                // Handle any exceptions thrown in the thread
                std::cerr << "Exception: " << e.what() << std::endl;
            }

            t.join();
        }
        CYAN("task6::end");

        MAGENTA("task6::exception::start");
        {
            // Create a promise to hold the result or exception
            std::promise<int> result_promise;
            
            // Get a future from the promise
            std::future<int> result_future = result_promise.get_future();
            
            // Start a thread and pass the promise
            std::thread t(task6_compute_factorial, -1, std::move(result_promise));
            
            try {
                // Get the result (blocks until the promise is fulfilled)
                int result = result_future.get();
                std::cout << "Factorial result: " << result << std::endl;
            } catch (const std::exception& e) {
                // Handle any exceptions thrown in the thread
                std::cerr << "Exception: " << e.what() << std::endl;
            }

            t.join();
        }
        CYAN("task6::exception::end");
    }
    
    MAGENTA("task7::shared_data::start");
    {
        std::mutex mtx; // Mutex for synchronization
        std::vector<int> sharedData;

        auto addToVector = [&](int value)
        {
            std::lock_guard<std::mutex> lock(mtx); // Protect shared resource
            sharedData.push_back(value);
            std::cout << "Added: " << value << "\n";
        };

        std::thread t1(addToVector, 1);
        std::thread t2(addToVector, 2);

        t1.join();
        t2.join();

        std::cout << "Final vector: ";
        for (int val : sharedData) {
            std::cout << val << " ";
        }
        std::cout << "\n";

    }
    CYAN("task7::shared_data::start");

    MAGENTA("task8::shared_data::start");
    {
        // Multiple Threads Working Together
        auto partialSum = [](const std::vector<int>& data, int start, int end, int& result)
        {
            result = std::accumulate(data.begin() + start, data.begin() + end, 0);
        };

        std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int mid = data.size() / 2;

        int sum1 = 0, sum2 = 0;
        std::thread t1(partialSum, std::cref(data), 0, mid, std::ref(sum1)); // First half
        std::thread t2(partialSum, std::cref(data), mid, data.size(), std::ref(sum2)); // Second half

        t1.join();
        t2.join();

        std::cout << "Total sum: " << (sum1 + sum2) << "\n";
    }
    CYAN("task8::shared_data::start");

    MAGENTA("task9::shared_data::start");
    {
        auto partialSum = [](const std::vector<int>& data, size_t start, size_t end, long long& result) {
            result = std::accumulate(data.begin() + start, data.begin() + end, 0LL);
        };

        const size_t size = 1'000'000;
        std::vector<int> data(size, 1); // Fill with 1s

        auto doSum = [partialSum, size, data](size_t numThreads)
        {
            size_t chunkSize = size / numThreads;

            std::vector<std::thread> threads;
            std::vector<long long> results(numThreads);

            for (size_t i = 0; i < numThreads; ++i) {
                size_t start = i * chunkSize;
                size_t end = (i == numThreads - 1) ? size : start + chunkSize;

                threads.emplace_back(partialSum, std::ref(data), start, end, std::ref(results[i]));
            }

            for (auto& t : threads) {
                t.join();
            }

            long long totalSum = std::accumulate(results.begin(), results.end(), 0LL);
            std::cout << "Total sum: " << totalSum << "\n";
        };

        Utils::time_elapsed(doSum, "1 thread", 1);
        Utils::time_elapsed(doSum, "2 thread", 2);
        Utils::time_elapsed(doSum, "3 thread", 3);
        Utils::time_elapsed(doSum, "4 thread", 4);
    }
    MAGENTA("task9::shared_data::end");

    return 0;
}

