const Qualifier
    The const qualifier makes an object read-only—its value cannot be modified after initialization.
volatile Qualifier
    The volatile qualifier tells the compiler that the value of a variable may change at any time,
    often due to external factors like hardware. This prevents the compiler from optimizing accesses to the variable.
Combining const and volatile
    A variable can be both const and volatile, meaning its value can change unexpectedly
    (like volatile) but cannot be directly modified (like const).

In C++, the mutable keyword is used to indicate that a member variable of a class can be modified
 even when the object itself is const. By default, in a const member function,
 you are not allowed to modify any member variables, because const implies that the object should remain unchanged.
 However, there are cases where you might want to allow modification of specific member variables even within const functions.
 This is where the mutable keyword comes in.

Key Points about mutable:
It allows a member variable to be modified even inside const member functions.
This can be useful for things like caching, lazy initialization, or flags that don't logically affect the object's state as observed by the user.