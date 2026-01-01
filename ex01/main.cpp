#include <stack>
#include <string>
#include <sstream>
#include <iostream>
#include <cctype>

static bool calculation(int left, int right, char op, int& out) {
    switch (op) {
        case '+': out = left + right; return true;
        case '-': out = left - right; return true;
        case '*': out = left * right; return true;
        case '/':
            if (right == 0) return false;
            out = left / right; return true;
        default: return false;
    }
}

static bool isOp(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int main() {
    std::stack<int> s;
    std::string expr = "8 9 * 9 -";
    std::istringstream iss(expr);
    std::string tok;

    while (iss >> tok) {
        if (tok.size() == 1 && std::isdigit(static_cast<unsigned char>(tok[0]))) {
            s.push(tok[0] - '0');
        } else if (tok.size() == 1 && isOp(tok[0])) {
            if (s.size() < 2) { std::cerr << "Error\n"; return 1; }
            int right = s.top(); s.pop();
            int left  = s.top(); s.pop();

            int result;
            if (!calculation(left, right, tok[0], result)) { std::cerr << "Error\n"; return 1; }
            s.push(result);
        } else {
            std::cerr << "Error\n";
            return 1;
        }
    }

    if (s.size() != 1) { std::cerr << "Error\n"; return 1; }
    std::cout << s.top() << "\n";
}
