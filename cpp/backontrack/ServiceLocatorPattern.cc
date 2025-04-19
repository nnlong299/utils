#include <iostream>
#include <unordered_map>
#include <memory>

// Abstract Service
class Service {
public:
    virtual void execute() = 0;
};

// Concrete Service
class ConcreteService : public Service {
public:
    void execute() override {
        std::cout << "Executing Concrete Service\n";
    }
};

// Service Locator
class ServiceLocator {
private:
    static std::unordered_map<std::string, std::shared_ptr<Service>> services;

public:
    static void registerService(const std::string& name, std::shared_ptr<Service> service) {
        services[name] = service;
    }

    static std::shared_ptr<Service> getService(const std::string& name) {
        return services[name];
    }
};

// Initialize static member
std::unordered_map<std::string, std::shared_ptr<Service>> ServiceLocator::services;

int main() {
    // Register service
    ServiceLocator::registerService("MyService", std::make_shared<ConcreteService>());

    // Retrieve and use service
    auto service = ServiceLocator::getService("MyService");
    service->execute();

    return 0;
}
