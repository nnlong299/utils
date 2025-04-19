#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

std::mutex mtx;                      // Mutex for synchronizing access to shared buffer
std::condition_variable cv_producer;  // Condition variable for the producer
std::condition_variable cv_consumer;  // Condition variable for the consumer

std::queue<int> buffer;              // Shared buffer
const int BUFFER_SIZE = 5;           // Maximum buffer size

// Producer function
void producer(int id) {
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simulate work
        std::unique_lock<std::mutex> lock(mtx);  // Lock the mutex to access the shared buffer

        // Wait if the buffer is full
        cv_producer.wait(lock, []{ return buffer.size() < BUFFER_SIZE; });

        // Produce an item and add it to the buffer
        buffer.push(i);
        std::cout << "Producer " << id << " produced: " << i << "\n";

        // Notify the consumer that an item has been produced
        cv_consumer.notify_one();
    }
}

// Consumer function
void consumer(int id) {
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Simulate work
        std::unique_lock<std::mutex> lock(mtx);  // Lock the mutex to access the shared buffer

        // Wait if the buffer is empty
        cv_consumer.wait(lock, []{ return !buffer.empty(); });

        // Consume an item from the buffer
        int item = buffer.front();
        buffer.pop();
        std::cout << "Consumer " << id << " consumed: " << item << "\n";

        // Notify the producer that there is space in the buffer
        cv_producer.notify_one();
    }
}

int main() {
    std::thread producer1(producer, 1);
    std::thread producer2(producer, 2);
    std::thread consumer1(consumer, 1);
    std::thread consumer2(consumer, 2);

    // Wait for all threads to finish
    producer1.join();
    producer2.join();
    consumer1.join();
    consumer2.join();

    return 0;
}
