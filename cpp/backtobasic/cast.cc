1. static_cast
Purpose: Performs compile-time conversions
Use Cases:
    Converting between compatible types, like int to float.
    Casting up and down a class hierarchy (when the relationship is well-defined and safe).
    {
        up-casting is always safe because derived class 'is-a' base class
        down-casting can be unsafe if you don't know what derived class is, using dynamic_cast instead
    }
    Removing qualifiers like volatile

2. dynamic_cast
Purpose: Performs runtime type checking and casting, primarily in polymorphic scenarios.
Use Cases:
    Checks if an object is of a certain type at runtime. {
        dynamic_cast uses type information stored in the runtime type information (RTTI)
        of polymorphic objects (classes with at least one virtual function) to verify the object''s type.
    }
    Safe downcasting in a class hierarchy using virtual inheritance.
    {
        When you are unsure of the actual type of a base class pointer or reference,
        you can use dynamic_cast to safely downcast it to a derived class type.
        dynamic_cast performs a runtime check to ensure the validity of the cast.
        If the cast is invalid, it returns nullptr for pointers or throws a std::bad_cast exception for references
        {
            pointer
            {
                int main() {
                    // Base pointers pointing to derived objects
                    std::vector<Base*> objects = {new DerivedA, new DerivedB, new DerivedA};

                    for (Base* obj : objects) {
                        // Safe downcast using dynamic_cast
                        if (DerivedA* derivedA = dynamic_cast<DerivedA*>(obj)) {
                            derivedA->display();
                        } else if (DerivedB* derivedB = dynamic_cast<DerivedB*>(obj)) {
                            derivedB->display();
                        } else {
                            std::cout << "Unknown type!\n";
                        }
                    }

                    // Clean up
                    for (Base* obj : objects) {
                        delete obj;
                    }

                    return 0;
                }
            }

            reference
            {
                void process(Base& obj) {
                    try {
                        // Safe downcast using dynamic_cast
                        DerivedA& derivedA = dynamic_cast<DerivedA&>(obj);
                        derivedA.display();
                    } catch (const std::bad_cast&) {
                        try {
                            DerivedB& derivedB = dynamic_cast<DerivedB&>(obj);
                            derivedB.display();
                        } catch (const std::bad_cast&) {
                            std::cout << "Unknown type!\n";
                        }
                    }
                }

            }
        }

        solution - using virtual
        {
            class Base {
            public:
                virtual void display() const = 0; // Pure virtual function
                virtual ~Base() {}
            };

            class DerivedA : public Base {
            public:
                void display() const override {
                    std::cout << "DerivedA display()\n";
                }
            };

            class DerivedB : public Base {
            public:
                void display() const override {
                    std::cout << "DerivedB display()\n";
                }
            };

        }
    }

3. const_cast
Purpose: Adds or removes const or volatile qualifiers.
Use Cases:
    Modifying a const object (use with caution and only when truly necessary).

4. reinterpret_cast
Purpose: Performs low-level, unsafe type conversions.
Use Cases:
    Casting between unrelated pointer types.
    Casting an integral type to a pointer and vice vers
