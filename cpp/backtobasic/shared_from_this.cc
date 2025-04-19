#include <iostream>
#include <memory>

class Component;

class Manager : public std::enable_shared_from_this<Manager> {
public:
    std::shared_ptr<Component> createComponent() {
        return std::make_shared<Component>(shared_from_this());
    }

    void notify() {
        std::cout << "Manager notified by component.\n";
    }
};

class Component {
public:
    explicit Component(std::shared_ptr<Manager> manager) : manager(std::move(manager)) {}

    void trigger() {
        manager->notify();
    }

private:
    std::shared_ptr<Manager> manager;
};

int main() {
    auto manager = std::make_shared<Manager>();
    auto component = manager->createComponent();
    std::cout << manager.use_count() << std::endl;
    component->trigger();
    return 0;
}



// this is a use case where worker goes out of scope and get destroyed
// but another thread doesn't know and try to use that object, then UB
//
// #include <iostream>
// #include <memory>
// #include <thread>
// #include <chrono>

// class Worker : public std::enable_shared_from_this<Worker> {
// public:
//     void startWork() {
//         auto self = shared_from_this(); // Keep the object alive
//         std::thread([self]() {
//             std::this_thread::sleep_for(std::chrono::seconds(2));
//             self->doWork();
//         }).detach();
//     }

//     void doWork() {
//         std::cout << "Work completed!" << std::endl;
//     }
// };

// int main() {
//     {
//         auto worker = std::make_shared<Worker>();
//         worker->startWork();
//     } // Worker object goes out of scope, but it stays alive until work is done

//     std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait for the work to complete
//     return 0;
// }
