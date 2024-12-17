// std::condition_variable in C++
//
// The std::condition_variable in C++ is a synchronization
// primitive used for thread communication. It allows threads
// to wait until they are notified to proceed. It is typically
// used in conjunction with a mutex to safely coordinate shared data between threads.

// Purpose
//      Used to block one or more threads until another thread signals (or notifies) that they can proceed.
//      Prevents busy-waiting by suspending the thread until a condition is met.

// Key Components
//      std::condition_variable::wait(): Puts the current thread into a blocked state until it is notified.
//      std::condition_variable::notify_one(): Wakes up one waiting thread.
//      std::condition_variable::notify_all(): Wakes up all waiting threads.
//      std::unique_lock<std::mutex>: Used with std::condition_variable to provide mutual exclusion.

#include "utils.h"

std::mutex mtx;             // Mutex for synchronization
std::condition_variable cv; // Condition variable for signaling

// for task1
bool done = false;          // Flag to indicate production is done
std::queue<int> dataQueue;  // Shared queue between threads

// for task2
bool ready = false;         // Shared flag

int main()
{
    MAGENTA("task1::start");
    {
        auto producer = []()
        {
            for (int i = 0; i < 5; ++i)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work
                // std::unique_lock<std::mutex> lock(mtx);
                {
                    // std::lock_guard<std::mutex> lock(mtx);
                    std::scoped_lock<std::mutex> lock(mtx);
                    dataQueue.push(i);
                    std::cout << "Produced: " << i << std::endl;
                }
                cv.notify_one(); // Notify one waiting thread
            }
            std::unique_lock<std::mutex> lock(mtx);
            done = true;
            // cv.notify_all(); // Notify all threads that we're done producing
        };

        auto consumer = []()
        {
            while (true)
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, []
                        { return !dataQueue.empty() || done; }); // Wait until data is available or done
                while (!dataQueue.empty())
                {
                    int value = dataQueue.front();
                    dataQueue.pop();
                    std::cout << "Consumed: " << value << std::endl;
                }
                if (done)
                    break; // Exit loop if production is complete
            }
        };

        std::thread prod(producer);
        std::thread cons(consumer);

        prod.join();
        cons.join();
    }
    CYAN("task1::end");

    MAGENTA("task2::start");
    {
        auto workerThread = [](int id)
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, []
                    { return ready; }); // Wait until ready is true
            std::cout << "Worker " << id << " is processing...\n";
        };

        std::thread t1(workerThread, 1);
        std::thread t2(workerThread, 2);

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work in main
        {
            std::lock_guard<std::mutex> lock(mtx);
            ready = true; // Signal threads to start
            std::cout << "Main thread signals workers to start...\n";
        }
        cv.notify_all(); // Notify all waiting threads

        t1.join();
        t2.join();

    }
    CYAN("task2::end");

    return 0;
}
