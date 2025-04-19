#include <iostream>

template <typename T, typename U>
auto add(T a, U b) -> decltype(a + b)
{
    return a + b;
}

template <typename T, typename U>
auto add(T a, U b)
{
    return a + b;
}

int main(int argc, char const *argv[])
{
    int x = 42;
    decltype(x) y;  
    // auto c;
    std::cout<<"y "<<y;
    /* code */
    return 0;
}
