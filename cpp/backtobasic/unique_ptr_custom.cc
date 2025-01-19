#include <iostream>

template <typename T>
class UniquePtr
{
private:
    T* m_rawPtr;
public:
    explicit UniquePtr(T* p = nullptr):
    m_rawPtr(p)
    {
        std::cout<<"Main ctor\n";
    }

    ~UniquePtr() { delete m_rawPtr; }

    // delete copy
    UniquePtr(const UniquePtr& p) = delete;
    UniquePtr& operator=(const UniquePtr& p) = delete;

    // move
    UniquePtr(UniquePtr&& p):
    m_rawPtr(p.m_rawPtr)
    {
        std::cout<<"Move ctor\n";
        p.m_rawPtr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& p)
    {
        std::cout<<"Move operator\n";
        if (this != &p)
        {
            delete m_rawPtr;
            m_rawPtr = p.m_rawPtr;
            p.m_rawPtr = nullptr;
        }

        return *this;
    }

    T& operator*() const    { return *m_rawPtr; }
    T* operator->() const   { return  m_rawPtr; }
    operator bool() const   { return  m_rawPtr != nullptr; }

    void reset(T* p = nullptr)
    {
        delete m_rawPtr;
        m_rawPtr = p;
    }
};

template <typename T, typename... Args>
UniquePtr<T> make_uniquee(Args&&... args)
{
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

int main()
{
    auto ptr = make_uniquee<int>(3);
    if (ptr) std::cout<<"Hello1 World\n";

    auto ptr2 = std::move(ptr);
    // auto ptr2 = ptr;
    if (ptr) std::cout<<"Hello2 World\n";
    if (ptr2) std::cout<<"Hello3 World " << *ptr2 <<"\n";

    ptr.reset(new int(4));
    if (!ptr) std::cout<<"Hello4 World\n";
    else std::cout<<"Hello5 World " << *ptr <<"\n";


    return 0;
}
