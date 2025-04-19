#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <chrono>

const int NUM_THREADS = 8;
const int NUM_TRANSACTIONS = 1000000;

// Atomic Account
struct AtomicAccount {
    std::atomic<int> balance{0};

    void deposit(int amount) {
        balance.fetch_add(amount, std::memory_order_relaxed);
    }

    void withdraw(int amount) {
        int current = balance.load(std::memory_order_relaxed);
        while (current >= amount) {
            if (balance.compare_exchange_weak(current, current - amount, std::memory_order_relaxed)) {
                break;
            }
        }
    }
};

// Mutex Account
struct MutexAccount {
    int balance = 0;
    std::mutex mtx;

    void deposit(int amount) {
        std::lock_guard<std::mutex> lock(mtx);
        balance += amount;
    }

    void withdraw(int amount) {
        std::lock_guard<std::mutex> lock(mtx);
        if (balance >= amount) {
            balance -= amount;
        }
    }
};

// Lock-Free Account (More Complex Example)
struct LockFreeAccount {
    std::atomic<int> balance{0};

    void deposit(int amount) {
        balance.fetch_add(amount, std::memory_order_relaxed);
    }

    void withdraw(int amount) {
        int current = balance.load(std::memory_order_relaxed);
        while (current >= amount) {
            if (balance.compare_exchange_weak(current, current - amount, std::memory_order_relaxed)) {
                break;
            }
        }
    }

    int get_balance() const {
        return balance.load(std::memory_order_relaxed);
    }
};

void atomic_worker(AtomicAccount& account) {
    for (int i = 0; i < NUM_TRANSACTIONS; ++i) {
        account.deposit(1);
        account.withdraw(1);
    }
}

void mutex_worker(MutexAccount& account) {
    for (int i = 0; i < NUM_TRANSACTIONS; ++i) {
        account.deposit(1);
        account.withdraw(1);
    }
}

void lock_free_worker(LockFreeAccount& account) {
    for (int i = 0; i < NUM_TRANSACTIONS; ++i) {
        account.deposit(1);
        account.withdraw(1);
    }
}

int main() {
    AtomicAccount atomic_acc;
    MutexAccount mutex_acc;
    LockFreeAccount lock_free_acc;

    // Atomic Test
    auto start_atomic = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> atomic_threads;
    for (int i = 0; i < NUM_THREADS; ++i) {
        atomic_threads.emplace_back(atomic_worker, std::ref(atomic_acc));
    }
    for (auto& t : atomic_threads) {
        t.join();
    }
    auto end_atomic = std::chrono::high_resolution_clock::now();
    std::cout << "Atomic Account Balance: " << atomic_acc.balance << "\n";

    // Mutex Test
    auto start_mutex = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> mutex_threads;
    for (int i = 0; i < NUM_THREADS; ++i) {
        mutex_threads.emplace_back(mutex_worker, std::ref(mutex_acc));
    }
    for (auto& t : mutex_threads) {
        t.join();
    }
    auto end_mutex = std::chrono::high_resolution_clock::now();
    std::cout << "Mutex Account Balance: " << mutex_acc.balance << "\n";

    // Lock-Free Test
    auto start_lock_free = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> lock_free_threads;
    for (int i = 0; i < NUM_THREADS; ++i) {
        lock_free_threads.emplace_back(lock_free_worker, std::ref(lock_free_acc));
    }
    for (auto& t : lock_free_threads) {
        t.join();
    }
    auto end_lock_free = std::chrono::high_resolution_clock::now();
    std::cout << "Lock-Free Account Balance: " << lock_free_acc.get_balance() << "\n";

    std::chrono::duration<double> atomic_time = end_atomic - start_atomic;
    std::chrono::duration<double> mutex_time = end_mutex - start_mutex;
    std::chrono::duration<double> lock_free_time = end_lock_free - start_lock_free;

    std::cout << "Atomic Time: " << atomic_time.count() << " seconds\n";
    std::cout << "Mutex Time: " << mutex_time.count() << " seconds\n";
    std::cout << "Lock-Free Time: " << lock_free_time.count() << " seconds\n";

    return 0;
}
