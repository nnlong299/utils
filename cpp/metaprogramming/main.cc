Template programming is a subset of metaprogramming in C++ that uses templates to create:

a. Generic algorithms and data structures: Code that works with any type.
b. Type-safe operations: Ensuring correctness across different data types.
c. Compile-time logic: Implementing decisions or computations during compilation.

1. Function Templates
{
    template <typename T>
    T add(T a, T b) 
    {
        return a + b;
    }
}

2. Class Templates
{
    template <typename T>
    class Vector
    {
        T* data;
        size_t size;
    public:
        explicit Vector(size_t n) : size(n), data(new T[n]) {}
        ~Vector() { delete[] data; }
    };
}

3. Template Specialization
{
    template <>
    class Vector<bool> { /* Specialized implementation for bool */ };
}

4. Variadic Templates (ellipsis/fold expression)
- Allows templates to accept an arbitrary number of arguments.
{
    template <typename... Args>
    void print(Args... args)
    {
        (std::cout << ... << args) << '\n';
    }
}

C++ Template Metaprogramming - Fibonacci Using Template Metaprogramming
{
    template <int N>
    struct Fibonacci {
        static const int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
    };

    template <>
    struct Fibonacci<1> {
        static const int value = 1;
    };

    template <>
    struct Fibonacci<0> {
        static const int value = 0;
    };

    int main() {
        constexpr int result = Fibonacci<10>::value; // Compile-time computation
        static_assert(result == 55, "Wrong Fibonacci value!");
    }
}


Modern Metaprogramming with C++

{
    1. constexpr: Enables writing metaprograms that compute at compile-time.
    {
        constexpr int factorial(int n)
        {
            return n <= 1 ? 1 : n * factorial(n - 1);
        }

        static_assert(factorial(5) == 120, "Factorial is incorrect!");
    }

    2. Type Traits (<type_traits>): A library of templates for type inspection and manipulation.
    {
        #include <type_traits>

        template <typename T>
        void printType()
        {
            if constexpr (std::is_integral_v<T>) {
                std::cout << "Integral type\n";
            } else {
                std::cout << "Non-integral type\n";
            }
        }

    }

    3. Concepts (C++20): Provides a clean way to constrain template parameters.

    {
        template <typename T>
        concept Integral = std::is_integral_v<T>;

        template <Integral T>
        T add(T a, T b)
        {
            return a + b;
        }
    }

    SFINAE (Substitution Failure Is Not An Error)
    Technique for enabling/disabling templates based on type traits

    {
        #include <type_traits>

        template <typename T>
        typename std::enable_if<std::is_integral<T>::value>::type
        process(T value) {
            std::cout << "Processing integral type: " << value << '\n';
        }

        template <typename T>
        typename std::enable_if<!std::is_integral<T>::value>::type
        process(T value) {
            std::cout << "Processing non-integral type: " << value << '\n';
        }

        int main() {
            process(42);       // Integral
            process(3.14);     // Non-integral
        }
    }
}

decltype and std::declval
{
    {
        int x = 42;
        decltype(x) y = 43; // y is an int
    }
    {
        template <typename T>
        auto get_size(const T& container) -> decltype(container.size()) {
            return container.size();
        }
    }
}



Advanced Pattern Matching with Templates
{
    #include <type_traits>
    #include <iostream>

    template <typename T>
    void process(T value, std::true_type) {
        std::cout << "Processing integral: " << value << '\n';
    }

    template <typename T>
    void process(T value, std::false_type) {
        std::cout << "Processing non-integral: " << value << '\n';
    }

    template <typename T>
    void process(T value) {
        process(value, std::is_integral<T>{});
    }

    int main() {
        process(42);   // Integral
        process(3.14); // Non-integral
    }
}

Perfect Forwarding
{
    template <typename T, typename... Args>
    void emplace(std::vector<T>& v, Args&&... args) {
        v.emplace_back(std::forward<Args>(args)...);
    }

}

CRTP (Curiously Recurring Template Pattern)
{
    // Avoid the runtime cost of virtual functions by performing polymorphism at compile-time.
    // No vtables or runtime dispatch.
    // Works well for small, performance-critical applications.
    {
        #include <iostream>

        template <typename Derived>
        class Shape {
        public:
            void draw() {
                static_cast<Derived*>(this)->draw();
            }
        };

        class Circle : public Shape<Circle> {
        public:
            void draw() {
                std::cout << "Drawing a Circle\n";
            }
        };

        class Square : public Shape<Square> {
        public:
            void draw() {
                std::cout << "Drawing a Square\n";
            }
        };

        int main() {
            Circle c;
            Square s;
            c.draw(); // Output: Drawing a Circle
            s.draw(); // Output: Drawing a Square
        }
    }

    // CRTP can enforce that derived classes implement certain methods.
    {
        template <typename Derived>
        class Interface {
        public:
            void execute() {
                static_cast<Derived*>(this)->doSomething();
            }
        };

        class Concrete : public Interface<Concrete> {
        public:
            void doSomething() {
                std::cout << "Concrete implementation of doSomething\n";
            }
        };

        // Uncommenting the next class will cause a compile-time error
        /*
        class Invalid : public Interface<Invalid> {
            // Missing doSomething()
        };
        */

        int main() {
            Concrete c;
            c.execute();
        }

    }

    // Mixins are used to add functionality to a class without modifying the base class.
    {
        #include <iostream>
        template <typename Derived>
        class LoggingMixin {
        public:
            void log(const std::string& message) {
                std::cout << "Log: " << message << "\n";
            }
        };

        class Application : public LoggingMixin<Application> {
        public:
            void run() {
                log("Application is running");
            }
        };

        int main() {
            Application app;
            app.run();
        }

    }

    Policy-Based Design
    {
        template <typename Policy>
        class Algorithm : public Policy {
        public:
            void execute() {
                this->step1();
                this->step2();
            }
        };

        struct PolicyA {
            void step1() { std::cout << "PolicyA Step1\n"; }
            void step2() { std::cout << "PolicyA Step2\n"; }
        };

        int main() {
            Algorithm<PolicyA> algo;
            algo.execute();
        }
    }

    // shared_ptr count?
    {
        #include <iostream>
        template <typename Derived>
        class Counter {
        private:
            inline static int count = 0;

        public:
            Counter() { ++count; }
            ~Counter() { --count; }

            static int getCount() { return count; }
        };

        class MyClass : public Counter<MyClass> {};

        int main() {
            MyClass a, b, c;
            std::cout << "Count: " << MyClass::getCount() << "\n"; // Output: Count: 3

            {
                MyClass d;
                std::cout << "Count: " << MyClass::getCount() << "\n"; // Output: Count: 4
            }

            std::cout << "Count: " << MyClass::getCount() << "\n"; // Output: Count: 3
        }

    }

    CRTP + Singleton
    {
        template <typename Derived>
        class Singleton {
        public:
            static Derived& instance() {
                static Derived instance;
                return instance;
            }

        protected:
            Singleton() = default;
        };

        class MySingleton : public Singleton<MySingleton> {
        public:
            void greet() { std::cout << "Hello from Singleton\n"; }
        };

        int main() {
            MySingleton::instance().greet();
        }

    }

    CRTP + State Pattern
    {
        template <typename Derived>
        class State {
        public:
            void handle() {
                static_cast<Derived*>(this)->handle();
            }
        };

        class ConcreteStateA : public State<ConcreteStateA> {
        public:
            void handle() { std::cout << "Handling State A\n"; }
        };

        class ConcreteStateB : public State<ConcreteStateB> {
        public:
            void handle() { std::cout << "Handling State B\n"; }
        };

        int main() {
            ConcreteStateA stateA;
            ConcreteStateB stateB;

            stateA.handle(); // Output: Handling State A
            stateB.handle(); // Output: Handling State B
        }

    }
}