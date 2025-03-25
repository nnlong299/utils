#include <string>
#include "utils.h"

// this can be called Explicit Object Parameter

class Cell
{
public:
    void setValue(double value)
    {
        m_value = value;
    }

    void setValue_23(this Cell& self, double value)
    {
        self.m_value = value;
    }

    std::string& f() &              {return m_name;}
    const std::string& f() const &  {return m_name;}
    std::string&& f() &&            {return std::move(m_name);}

    template <typename Self>
    void SetName_23(this Self&& self, std::string_view name)
    {
        self.m_name = name;
    }

    template <typename Self>
    auto&& GetName_23(this Self&& self)
    {
        return std::forward<Self>(self).m_name;
    }

private:
    double m_value{};
    std::string m_name;
};

auto main() -> int 
{
    Cell cell;
    cell.setValue(1.2);
    cell.setValue_23(1.2);
    cell.SetName_23("wtf");
    RED(cell.GetName_23());

    auto fibo = [](this auto self, int n)
    {
        if (n < 2) {return n;}
        return self(n-1) + self(n-2);
    };

    YELLOW(fibo(6));
    return 0;
}

