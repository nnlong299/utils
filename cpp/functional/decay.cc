#include <type_traits>
#include <iostream>
 
template<typename T, typename U>
constexpr bool is_decay_equ = std::is_same_v<std::decay_t<T>, U>;
 
int main()
{
    // std::cout << "0: "  << std::boolalpha <<  std::decay_t<int>()                     << "\n";
    std::cout << "1: "  << std::boolalpha <<  is_decay_equ<int, int>                << "\n";
    std::cout << "2: "  << std::boolalpha <<  is_decay_equ<int, float>              << "\n";
    std::cout << "3: "  << std::boolalpha <<  is_decay_equ<int&, int>               << "\n";
    std::cout << "4: "  << std::boolalpha <<  is_decay_equ<int&&, int>              << "\n";
    std::cout << "5: "  << std::boolalpha <<  is_decay_equ<const int&, int>         << "\n";
    std::cout << "6: "  << std::boolalpha <<  is_decay_equ<int[2], int*>            << "\n";
    std::cout << "7: "  << std::boolalpha <<  is_decay_equ<int[4][2], int*>         << "\n";
    std::cout << "8: "  << std::boolalpha <<  is_decay_equ<int[4][2], int**>        << "\n";
    std::cout << "9: "  << std::boolalpha <<  is_decay_equ<int[4][2], int(*)[2]>    << "\n";
    std::cout << "10: " << std::boolalpha <<  is_decay_equ<int(int), int(*)(int)>   << "\n";
}