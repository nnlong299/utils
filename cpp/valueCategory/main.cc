int main()
{
    // Lvalue (Left Value) - Key Properties:
    //
    // Has a persistent memory location.
    // Can bind to T& (non-const lvalue reference).

    // Rvalue (Right Value) - Key Properties:
    //
    // Temporary and short-lived.
    // Can bind to T&& (rvalue reference).

    // Glvalue (Generalized Lvalue) - Key Properties:
    // Provides an object identity (can be located in memory).

    // Prvalue (Pure Rvalue) - Key Properties:
    // Cannot be directly assigned to.
    // May be optimized away (e.g., by copy elision).

    // Xvalue (Expiring Value) 
    // Represents an object whose resources can be "moved from" (i.e., about to expire).
    // Key Properties:
    //
    // Can bind to T&& (rvalue reference).
    // Allows moving resources instead of copying.
    //
    // Example : this is RVO (Return Value Optimization)
    // std::string foo() {
    //     return std::string("temp");
    // }
    // std::string s = foo();  // `foo()` returns an xvalue

    // Rvalue Reference
    // A reference type (T&&) that binds to rvalues and xvalues.
    // Key Use Cases:
    // Enable move semantics.
    // Perfect forwarding in templates.

    // Perfect Forwarding in C++ refers to the ability of a function template
    // to pass its arguments to another function without altering the value category
    // (whether the argument is an lvalue or rvalue). This is crucial for
    // generic programming, allowing templates to work seamlessly with all kinds of arguments.

    // Perfect forwarding ensures that:
    //
    // Lvalues remain lvalues when forwarded.
    // Rvalues remain rvalues when forwarded.

        #include <iostream>
        #include <string>

        void process(std::string& str) {
            std::cout << "Lvalue reference: " << str << "\n";
        }   

        void process(std::string&& str) {
            std::cout << "Rvalue reference: " << str << "\n";
        }

        template <typename T>
        void wrapper(T&& arg) {
            process(arg); // Always calls process(std::string&) due to type decay
            // process(std::forward<T>(arg)); // Preserves value category
        }

        int main() {
            std::string s = "Hello";
            wrapper(s);            // Calls process(std::string&)
            wrapper(std::move(s)); // Still calls process(std::string&) -- incorrect!
        }


    return 0;
}

