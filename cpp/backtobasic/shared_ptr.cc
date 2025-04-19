#include <iostream>
#include <memory>


class Base
{
public:
    Base() { std::cout << "Base()" << std::endl; }
    ~Base() { std::cout << "~Base()" << std::endl; }
    Base(const Base&) { std::cout << "Base(const Base&)" << std::endl; }
    Base& operator=(const Base&) { std::cout << "Base& operator=(const Base&)" << std::endl; return *this; }
    Base(Base&&) { std::cout << "Base(Base&&)" << std::endl; }
    Base& operator=(Base&&) { std::cout << "Base& operator=(Base&&)" << std::endl; return *this; }
    int var;
};

template <typename T>
class Derived : public Base
{
public:
    Derived() { std::cout << "Derived()" << std::endl; }
    ~Derived() { std::cout << "~Derived()" << std::endl; }
    Derived(const Derived&) { std::cout << "Derived(const Derived&)" << std::endl; }
    Derived& operator=(const Derived&) { std::cout << "Derived& operator=(const Derived&)" << std::endl; return *this; }
    Derived(Derived&&) { std::cout << "Derived(Derived&&)" << std::endl; }
    Derived& operator=(Derived&&) { std::cout << "Derived& operator=(Derived&&)" << std::endl; return *this; }
};

int use_shared_ptr(std::shared_ptr<Base> p)
{
    return p->var;
}

template <typename T>
std::shared_ptr<Base> d_factory()
{
    return std::make_shared<Derived<T>>();
}

int main()
{
    // std::shared_ptr<Base> p = std::make_shared<Derived>();
    d_factory<int>();
    return 0;
}