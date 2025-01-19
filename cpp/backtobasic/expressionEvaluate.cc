#include <iostream>
#include <stack>
#include <string>
#include <tuple>
#include <cctype>
#include <stdexcept>

auto get(std::stack<int>& values, std::stack<char>& ops) -> std::tuple<int, int, char>
{
    int b = values.top(); values.pop();
    int a = values.top(); values.pop();
    char op = ops.top(); ops.pop();
    return std::make_tuple(a,b,op);
} 
// Function to get precedence of operators
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Function to perform arithmetic operations
int applyOperator(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) throw std::invalid_argument("Division by zero");
            return a / b;
        default: throw std::invalid_argument("Unknown operator");
    }
}

// Function to evaluate infix expression
int evaluateInfix(const std::string& infix) {
    std::stack<int> values;  // Stack to store operands (numbers)
    std::stack<char> ops;    // Stack to store operators

    std::string number;  // To accumulate multi-digit numbers

    for (char ch : infix) {
        if (isdigit(ch)) {
            number += ch;  // Accumulate digits to form the full number
        } else {
            if (!number.empty()) {
                values.push(std::stoi(number));  // Push the number to the operand stack
                number.clear();
            }

            if (ch == '(') {
                ops.push(ch);  // Push left parenthesis to the operator stack
            } else if (ch == ')') {
                while (!ops.empty() && ops.top() != '(') {
                    auto [a,b,op] = get(values, ops);
                    values.push(applyOperator(a, b, op));  // Apply the operator
                }
                ops.pop();  // Pop the left parenthesis
            } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
                while (!ops.empty() && precedence(ops.top()) >= precedence(ch)) {
                    auto [a,b,op] = get(values, ops);
                    values.push(applyOperator(a, b, op));  // Apply the operator
                }
                ops.push(ch);  // Push the current operator to the stack
            }
        }
    }

    // Push the last number if any
    if (!number.empty()) {
        values.push(std::stoi(number));
    }

    // Perform remaining operations
    while (!ops.empty()) {
        auto [a,b,op] = get(values, ops);
        values.push(applyOperator(a, b, op));  // Apply the operator
    }

    // The result is the last remaining value in the stack
    return values.top();
}

int main() {
    try {
        std::string infix = "10 + 5 * (2 - 8)";
        std::cout << "Infix: " << infix << std::endl;

        // Evaluate the infix expression directly
        int result = evaluateInfix(infix);
        std::cout << "Result: " << result << std::endl; // Expected: -20
    } catch (const std::exception& ex) {
        std::cout << "Error: " << ex.what() << std::endl;
    }

    return 0;
}
