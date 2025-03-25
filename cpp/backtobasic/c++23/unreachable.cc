#include <iostream>
#include <utility>  // For std::unreachable()
#include "utils.h"

enum class Color { RED, GREEN, BLUE };

std::string getColorName(Color color) {
    switch (color) {
        case Color::RED:   return "Red";
        case Color::GREEN: return "Green";
        case Color::BLUE:  return "Blue";
    }
    std::unreachable();  // Compiler knows this point should never be reached
}

int main() {
    std::cout << getColorName(Color::RED) << "\n";
    // std::vector<std::pair<int, int>> v{{1,2}};
    // std::vector<int> v{1,2};
    // std::println("{}", v);
    return 0;
}
