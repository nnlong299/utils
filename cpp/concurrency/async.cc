// template : std::future<R> std::async(std::launch policy, Callable&& func, Args&&... args);

// std::launch policy:
//      std::launch::async: Forces the callable to execute asynchronously in a separate thread.
//      std::launch::deferred: Callable is executed lazily (only when future.get() or future.wait() is called).
//      std::launch::async | std::launch::deferred: Leaves it to the implementation to decide.
// Callable: The function, lambda, or functor to execute.
// Args...: Arguments to pass to the callable.

#include "utils.h"

int heavyComputation() {
    std::cout << "Heavy computation started.\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 42;
}

int main()
{
    MAGENTA("task1::start"); // blocking
    {
        auto asyncLambda = [](int x, int y)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate delay
            return x * y;
        };

        std::future<int> result = std::async(std::launch::async, asyncLambda, 4, 5);

        std::cout << "Calculating asynchronously...\n";
        std::cout << "Result: " << result.get() << "\n"; // blocking way
    }
    CYAN("task1::end");

    MAGENTA("task2::start");// blocking
    {
        std::future<int> result = std::async(std::launch::deferred, heavyComputation);

        std::cout << "Task created, but not started.\n";

        // Trigger the execution
        std::cout << "Result: " << result.get() << "\n"; // blocking way
    }
    CYAN("task2::end");

    MAGENTA("task3::start-non-blocking"); // non-blocking
    {
        auto longComputation = []()
        {
            std::this_thread::sleep_for(std::chrono::seconds(3)); // Simulate work
            return 42;
        };

        CYAN("1st solution");
        // Non-Blocking Alternatives:
        //      Use wait_for or wait_until to periodically check status.
        //      Use polling if periodic updates are necessary.
        {
            std::future<int> result = std::async(std::launch::async, longComputation);

            // Non-blocking check
            while (result.wait_for(std::chrono::milliseconds(500)) != std::future_status::ready) {
                std::cout << "Waiting for result...\n";
            }

            std::cout << "Result is ready: " << result.get() << "\n"; 
        }

        CYAN("2nd solution");
        // Avoid get if Not Needed:
        //      If the result is not required, consider skipping get and handling the task asynchronously.
        //      Be cautious of exceptions if you detach the future.
        {
            std::future<int> result = std::async(std::launch::async, longComputation);
            while (true)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Perform other work
                auto status = result.wait_for(std::chrono::milliseconds(0)); // Non-blocking check
                if (status == std::future_status::ready) {
                    std::cout << "Computation finished with result: " << result.get() << "\n";
                    break;
                } else {
                    std::cout << "Still working...\n";
                }
            }
        }

        CYAN("3rd solution");
        // Shared Results:
        //      Use std::shared_future if multiple consumers need access to the result.
        {
            std::shared_future<int> result = std::async(std::launch::async, longComputation).share();

            std::thread worker1([result] {
                std::cout << "Worker 1 sees result: " << result.get() << "\n";
            });

            std::thread worker2([result] {
                std::cout << "Worker 2 sees result: " << result.get() << "\n";
            });

            worker1.join();
            worker2.join();
        }

        CYAN("4th solution");
        // Fire-and-Forget (like J97):
        //      Suitable for tasks where you don’t need the result, but exceptions will not be handled.
        {
            (void)std::async(std::launch::async, longComputation); // Fire and forget

            std::cout << "Main thread continues working...\n";
            std::this_thread::sleep_for(std::chrono::seconds(3)); // Simulate other work
            std::cout << "Main thread finished.\n";
        }
    }
    CYAN("task3::end-non-blocking");

    MAGENTA("task4::start-exception"); // non-blocking
    {
        auto riskyComputation = [](bool throwError)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate some work
            if (throwError) {
                throw std::runtime_error("An error occurred during computation.");
            }
            return 42;
        };

        try
        {
            // Launch a task that might throw an exception
            std::future<int> result = std::async(std::launch::async, riskyComputation, true);

            // This will rethrow any exception from the task
            int value = result.get();  // This line will throw the exception from riskyComputation

            std::cout << "Computation result: " << value << std::endl;  // Will not be reached
        }
        catch (const std::exception& e)
        {
            std::cout << "Caught exception: " << e.what() << std::endl;
        }
    }
    CYAN("task4::end-exception");

    return 0;
}
