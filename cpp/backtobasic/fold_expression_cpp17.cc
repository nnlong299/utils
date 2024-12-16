
#include<iostream>
#include <climits>
#include <concepts>
#include <cstdint>
#include <iostream>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

// Fold expression in C++17
// The syntax (expr1 op expr2 op ... op exprN) is the general format
// where op is the binary operator applied between each argument in the pack.



template<typename... Args>
int sum(Args&&... args)
// Args&&... args: This is a parameter pack that allows the function to accept any number of arguments of any type.
// The && in Args&& is a forwarding reference (also known as "universal reference"), which can bind to both rvalues and lvalues.
{
//  return (args + ... + 1 * 2);   // Error: operator with precedence below cast
    return (args + ...); // OK
}

// Basic usage, folding variadic arguments over operator<< 
template<typename... Args>
void printer(Args&&... args)
{
    (std::cout << ... << args) << '\n';
}

// Folding an expression that uses the pack directly over operator,
template<typename... Ts>
void print_limits()
{
    ((std::cout << +std::numeric_limits<Ts>::max() << ' '), ...) << '\n';
}

// Both a fold over operator&& using the pack
// and over operator, using the variadic arguments
template<typename T, typename... Args>
void push_back_vec(std::vector<T>& v, Args&&... args)
{
    static_assert((std::is_constructible_v<T, Args&&> && ...));
    // The && ... part is a fold expression that applies the logical AND (&&)
    // across all of the std::is_constructible_v<T, Args&&> checks for each argument in Args.
    // It is essentially saying, "check if each of the Args types can be used to construct T."
    
    (v.push_back(std::forward<Args>(args)), ...); 
    // is a fold expression that iterates over each argument in args and calls v.push_back(std::forward<Args>(args)) for each one.
    // std::forward<Args>(args) is used for perfect forwarding. It ensures that each argument is forwarded with its original value category (whether it was an lvalue or rvalue) to push_back. This is important for efficiency, as it avoids unnecessary copies.
    // If the argument is an rvalue, it will be moved (avoiding a copy).
    // If the argument is an lvalue, it will be copied (if necessary).
    // The , ... syntax is a fold expression over the comma operator. The fold expression applies the comma operator to each argument, effectively calling push_back for each one.
}

// Using an integer sequence to execute an expression
// N times by folding a lambda over operator,
template<class T, std::size_t... dummy_pack>
constexpr T bswap_impl(T i, std::index_sequence<dummy_pack...>)
{
    T low_byte_mask = static_cast<unsigned char>(-1);
    T ret{};
    ([&]
    {
        (void)dummy_pack; // doesnot involve to real process but the times of fold expression repeats
        ret <<= CHAR_BIT;
        ret |= i & low_byte_mask;
        i >>= CHAR_BIT;
    }(), ...);
    return ret;
}
 
constexpr auto bswap(std::unsigned_integral auto i)
{
    // the sequence is how many time the fold expression executes
    return bswap_impl(i, std::make_index_sequence<sizeof(i)>{});
}

template<typename... Args>
bool all_true(Args&&... args)
{
    return (... && std::forward<Args>(args)); 
    // Logical AND of all arguments
    // ensures that all arguments are checked with a logical AND, and the evaluation happens across all arguments.

    // within all(), the unary left fold expands as
    //  return ((true && true) && true) && false;
    // b is false

    // vs std::all_of
    // {
    //     When to Use Each:
    //         Use all_true():
    //         When you have a fixed number of boolean arguments.
    //         When you want a lightweight, specialized solution to check logical conditions.
    //         Use std::all_of():
    //         When you're working with iterable containers (e.g., std::vector, std::array, std::list, etc.).
    //         When you need to apply a custom condition using a predicate function or lambda.
    //         When you're working with standard algorithms for more general use cases.
    // }
}

template<typename... Args>
bool any_true(Args&&... args)
{
    return (... || std::forward<Args>(args));  // Check if any argument is true
}

template<typename... Args>
std::string concat(Args&&... args)
{
    return (std::string{} + ... + std::forward<Args>(args));  // Concatenate all strings
}

template<typename T, typename... Args>
std::vector<T> concatenate_vectors(Args&&... args)
{
    std::vector<T> result;
    (result.insert(result.end(), std::forward<Args>(args).begin(), std::forward<Args>(args).end()), ...);
    return result;
}

int main() {
    // fold expression since  C++17 only
    printer(sum(1,2,3,4,5), '\n');
    print_limits<uint8_t, uint16_t, uint32_t, uint64_t>();
    std::cout << concat("Hello, ", "world", "!") << std::endl;
    std::cout << std::boolalpha << all_true(true, true, true)  << std::endl;
    std::vector<int> v1 = {1, 2};
    std::vector<int> v2 = {3, 4};
    std::vector<int> v3 = {5, 6};
    auto combined = concatenate_vectors<int>(v1, v2, v3);
    return 0;
}
