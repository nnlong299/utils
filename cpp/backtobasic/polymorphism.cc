Polymorphism
{
    class Base {
    public:
        virtual void show() { // Virtual function
            std::cout << "Base class show()\n";
        }
        virtual ~Base() {} // Virtual destructor (important for cleanup in polymorphic use)
    };

    class Derived : public Base {
    public:
        void show() override { // Overrides Base::show
            std::cout << "Derived class show()\n";
        }
    };

    int main() {
        Base* basePtr; // Pointer to base class
        Derived derivedObj;
        
        basePtr = &derivedObj; // Base pointer points to Derived object
        basePtr->show();       // Calls Derived's show() due to polymorphism

        return 0;
    }

    int main() {
        Derived derivedObj;
        Base& baseRef = derivedObj; // Base reference refers to Derived object

        baseRef.show(); // Calls Derived's display() due to polymorphism

        return 0;
    }
}

Extra
{
    int main() {
        // Using smart pointers for polymorphic storage
        std::vector<std::unique_ptr<Base>> container;

        // Add instances of DerivedA and DerivedB
        container.push_back(std::make_unique<DerivedA>());
        container.push_back(std::make_unique<DerivedB>());

        // Iterate and call display()
        for (const auto& obj : container) {
            obj->display(); // Polymorphic behavior
        }

        return 0;
    }

    int main() {
        DerivedA objA;
        DerivedB objB;

        // Using std::reference_wrapper for references
        std::vector<std::reference_wrapper<Base>> container;

        // Add references to the container
        container.push_back(objA);
        container.push_back(objB);

        // Iterate and call display()
        for (const auto& obj : container) {
            obj.get().display(); // Polymorphic behavior
        }

        return 0;
    }

    When to Use Which?
        Use pointers (preferably smart pointers) if you need dynamic allocation or ownership management.
        Use references if objects are already created and managed elsewhere, and you just want to group them temporarily.
}

Note
{
    if virtual keyword is not defined in method
    despite of base ref/ptr to derived, base still doesnot
    see the part that extend in derived class, because if virtual
    is there, c++ will help look for 'override' between classes
    and judge if method is overrided and call with correct function

    
    Why Use std::reference_wrapper?
    Works with Containers:
        std::reference_wrapper provides the required properties (assignable and copyable) to store references in containers.
    Reference Semantics:
        Even though it is wrapped, it behaves like a reference. Any changes to the original object are reflected through the wrapper.
    Safe and Intuitive:
        The .get() method makes it explicit that you are dealing with a wrapped reference.

    When to Use std::reference_wrapper
        Use std::reference_wrapper when you need to:
        Group existing objects (which are already managed elsewhere) into a container.
        Avoid copying objects into the container.
        Work with references while leveraging STL container semantics.
}