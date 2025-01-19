#include <iostream>

template <typename T>
class SharedPtr
{
private:
    T* m_rawPtr;
    size_t* refCount;

public:
    explicit SharedPtr(T* other = nullptr):
    m_rawPtr(other),
    refCount(new size_t(1))
    {
        std::cout<<"main ctor\n";
    }

    ~SharedPtr()
    {
        release();
    }

    SharedPtr(SharedPtr& other):
    m_rawPtr(other.m_rawPtr),
    refCount(other.refCount)
    {
        ++(*refCount);
    }

    SharedPtr& operator=(SharedPtr& other)
    {
        std::cout<<"copy assignment\n";
        if (this != &other)
        {
            std::cout<<"copy ctor\n";
            m_rawPtr = other.m_rawPtr;
            refCount = other.refCount;
            ++(*refCount);
        }
        return *this;
    }

    void reset(T* other = nullptr)
    {
        if (m_rawPtr != other)
        {
            release();
            m_rawPtr = other;
            if (m_rawPtr) 
            {
                refCount = new size_t(1);
            }
            else
            {
                refCount = nullptr;
            }
        }
    }

    size_t getRefCount() const
    {
        return *refCount;
    }

    operator bool() const {return m_rawPtr!=nullptr;}
    T* operator->() const {return m_rawPtr;}
    T& operator*() const {return *m_rawPtr;}
private:
    void release()
    {
        if(refCount && --(*refCount) == 0)
        {
            delete m_rawPtr;
            delete refCount;
        }
    }
};

int main()
{
    auto ptr = SharedPtr( new int(3));
    if (ptr) std::cout<<"Hello1 World\n";
    std::cout<<"Count "<< ptr.getRefCount() << " " << ptr << "\n";
    {
        auto ptr2 = ptr;
        ptr.reset();
        if (ptr2) std::cout<<"Hello2 World\n";
        std::cout<<"Count2 "<< ptr2.getRefCount() << "\n";
        // std::cout<<"Count "<< ptr.getRefCount()  << "\n";
        // ptr.reset();
    }
    if (!ptr) std::cout<<"Hello3 World\n";



    return 0;
}
