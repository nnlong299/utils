#include <iostream>
#include <stack>
#include <string>
#include <tuple>
#include <cctype>
#include <stdexcept>

auto get(std::stack<double> &values, std::stack<char> &ops) -> std::tuple<double, double, char>
{
    auto b  = values.top();     values.pop();
    auto a  = values.top();     values.pop();
    auto op = ops.top();        ops.pop();
    return std::make_tuple(a, b, op);
}
// Function to get precedence of operators
int precedence(char op)
{
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Function to perform arithmetic operations
double applyOperator(double a, double b, char op)
{
    switch (op)
    {
    case '+':
        std::cout<< a << " + " << b << " = " << a + b<<"\n";
        return a + b;
    case '-':
        std::cout<< a << " - " << b << " = " << a - b<<"\n";
        return a - b;
    case '*':
    {
        auto result = a * 1.0 * b;
        std::cout<< a << " * " << b << " = " << result<< "\n";
        return result;
    }
    case '/':
    {
        if (b == 0)
            throw std::invalid_argument("Division by zero");
        auto result = a * 1.0 / b;
        std::cout<< a << " / " << b << " = " << result<<"\n";
        return result;
    }
    default:
        throw std::invalid_argument("Unknown operator");
    }
}

// Function to evaluate infix expression
double evaluateInfix(const std::string &infix)
{
    std::stack<double> values; // Stack to store operands (numbers)
    std::stack<char> ops;   // Stack to store operators

    std::string number; // To accumulate multi-digit numbers

    for (char ch : infix)
    {
        if (isdigit(ch))
        {
            number += ch; // Accumulate digits to form the full number
            continue;
        }

        if (!number.empty())
        {
            values.push(std::stod(number)); // Push the number to the operand stack
            number.clear();
        }

        switch (ch) {
        case '(': 
            ops.push(ch); // Push left parenthesis to the operator stack
            break;

        case ')':
            while (!ops.empty() && ops.top() != '(') {
                auto [a, b, op] = get(values, ops);
                values.push(applyOperator(a, b, op)); // Apply the operator
            }
            ops.pop(); // Pop the left parenthesis
            break;

        case '+':
        case '-':
        case '*':
        case '/':
            while (!ops.empty() && precedence(ops.top()) >= precedence(ch)) {
                auto [a, b, op] = get(values, ops);
                values.push(applyOperator(a, b, op)); // Apply the operator
            }
            ops.push(ch); // Push the current operator to the stack
            break;

        case ' ':
            // space is ok
            break;

        default:
            std::cerr << "'Unknown input " << ch << "'" << std::endl;
            throw std::invalid_argument("Unknown input");
    }
    }

    if (!number.empty()) // input "100"
    {
        values.push(std::stod(number));
    }

    // Perform remaining operations
    while (!ops.empty())
    {
        auto [a, b, op] = get(values, ops);
        values.push(applyOperator(a, b, op)); // Apply the operator
    }

    // The result is the last remaining value in the stack
    return values.top();
}

int main()
{
    try
    {
        std::string infix = "100";
        std::cout << "Infix: " << infix << std::endl;

        // Evaluate the infix expression directly
        int result = evaluateInfix(infix);
        std::cout << "Result: " << result << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }

    return 0;
}
