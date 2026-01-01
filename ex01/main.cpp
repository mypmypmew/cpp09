#include "RPN.hpp"
#include <sstream>
#include <iostream>
#include <string>
#include <cctype>

int main() {
    std::string expr = "8 9 * 9 -";
    std::istringstream iss(expr);
    std::string tok;

    while (iss >> tok) {
        if (tok.size() == 1 && std::isdigit(tok[0])) {
            std::cout << "NUMBER: " << tok << "\n";
        }
        else if (tok.size() == 1 &&
                 (tok[0] == '+' || tok[0] == '-' ||
                  tok[0] == '*' || tok[0] == '/')) {
            std::cout << "OPERATOR: " << tok << "\n";
        }
        else {
            std::cout << "ERROR TOKEN: " << tok << "\n";
        }
    }
}