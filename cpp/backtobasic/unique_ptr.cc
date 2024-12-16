Key Features of std::unique_ptr:
{
    Exclusive ownership: Only one unique_ptr can own the resource at a time.
    Move-only semantics: It cannot be copied, only moved, ensuring unique ownership.
}
Constructors:
{
    Default constructor: unique_ptr()
    Creates an empty unique_ptr (does not own any resource).

    Pointer constructor: explicit unique_ptr( pointer p )
    Takes ownership of a pointer (p).

    Custom deleter constructor:
    unique_ptr( pointer p, Deleter d )
    Takes ownership of a pointer (p) with a custom deleter (d).

    Move constructor:
    unique_ptr( unique_ptr&& u ) noexcept
    Transfers ownership from another unique_ptr (u).
}

Destructor:
{
    ~unique_ptr()
    Destroys the managed object by invoking the deleter.
}

Assignment:
{
    Move assignment:
    unique_ptr& operator=( unique_ptr&& u ) noexcept
    Transfers ownership from another unique_ptr (u).

    Reset assignment:
    void reset( pointer p = pointer() ) noexcept
    Releases the owned object and optionally takes ownership of a new object.
}

Modifiers:
{
    reset:
    Releases the ownership of the managed object, optionally replacing it with another.

    release:
    Releases ownership and returns the raw pointer without deleting it. The unique_ptr becomes empty.

    swap:
    Exchanges ownership with another unique_ptr.
}
Observers:
{
    get:
    Returns the raw pointer to the managed object without releasing ownership.

    operator bool:
    Checks whether the unique_ptr is not empty (i.e., owns a resource).
}
Custom Deleter Access:
{
    get_deleter:
    Returns a reference to the deleter used by the unique_ptr.
}