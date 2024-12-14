// only work with:
// + std::function
// + std::bind
// + lambda expression
// don't talk about C style here

#include <functional>
#include <iostream>
#include <iomanip>
 
struct Foo
{
    explicit Foo(int num) : num_(num) {}
    void print_add1(int i) const 
    {
        std::cout << num_ + i << '\n'; 
    }
    void print_add2(int i, int& j) 
    {
        std::cout << &i << " "<< num_ + i << '\n'; 
        std::cout << &j << " "<< ++j << '\n'; 
    }
    int num_;
};
 
void print_num(int i)
{
    std::cout << i << '\n';
}
 
struct PrintNum
{
    void operator()(int i) const
    {
        std::cout << i << '\n';
    }
};
 
int main()
{
    // // store a free function
    // std::function<void(int)> f_display = print_num;
    // f_display(-9);
 
    // // store a lambda
    // std::function<void()> f_display_42 = []() { print_num(42); };
    // f_display_42();
 
    // // store the result of a call to std::bind
    // std::function<void()> f_display_31337 = std::bind(print_num, 31337);
    // f_display_31337();
 
    // // store a call to a member function
    // std::function<void(const Foo&, int)> f_add_display = &Foo::print_add1;
    const Foo foo(314159);
    // f_add_display(foo, 1);

    // store the result of a call to std::bind
    int i = 2;
    int j = 3;
    std::cout << std::quoted("out")<< " " << &i << " "<<  i << '\n'; 
    std::cout << std::quoted("out")<< " " << &j << " "<<  j << '\n'; 
    auto f_add_display2 = std::bind(&Foo::print_add2, foo, i, std::ref(j));
    f_add_display2();

    // this will implicitly convert 'int' to 'const Foo&'
    // bad practice always
    // solution is to 'explicit' at ctor
    // f_add_display(314159, 1);
 
    // // store a call to a data member accessor
    // std::function<int(Foo const&)> f_num = &Foo::num_;
    // std::cout << "num_: " << f_num(foo) << '\n';
 
    // // store a call to a member function and object
    // using std::placeholders::_1;
    // std::function<void(int)> f_add_display2 = std::bind(&Foo::print_add, foo, _1);
    // f_add_display2(2);
 
    // // store a call to a member function and object ptr
    // std::function<void(int)> f_add_display3 = std::bind(&Foo::print_add, &foo, _1);
    // f_add_display3(3);
 
    // // store a call to a function object
    // std::function<void(int)> f_display_obj = PrintNum();
    // f_display_obj(18);
 
    // auto factorial = [](int n)
    // {
    //     // store a lambda object to emulate "recursive lambda"; aware of extra overhead
    //     std::function<int(int)> fac = [&](int n) { return (n < 2) ? 1 : n * fac(n - 1); };
    //     // note that "auto fac = [&](int n) {...};" does not work in recursive calls
    //     return fac(n);
    // };
    // for (int i{5}; i != 8; ++i)
    //     std::cout << i << "! = " << factorial(i) << ";  ";
    // std::cout << '\n';
}