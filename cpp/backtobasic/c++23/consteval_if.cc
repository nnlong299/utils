#include "utils.h"

consteval int f(int i)
{
    return i;
}

constexpr int compute(int i)
{
    if consteval
    {
        return f(i)+1;
    }
    else
    {
        return 42;
    }
}


auto main() -> int
{
    constexpr int a = compute(5);  // Calls compile-time version (5 * 2)
    int b = compute(5);  // Calls runtime version (5 * 3)

    std::cout << "Compile-time result: " << a << "\n";
    std::cout << "Runtime result: " << b << "\n";

    return 0;
}
