#include "RPN.hpp"

#include <stack>
#include <sstream>
#include <cctype>
#include <stdexcept>

RPN::RPN() {}
RPN::RPN(const RPN& other) { (void)other; }
RPN& RPN::operator=(const RPN& other) { (void)other; return *this; }
RPN::~RPN() {}

static bool isOp(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

static long long applyOp(long long left, long long right, char op) {
    switch (op) {
        case '+': return left + right;
        case '-': return left - right;
        case '*': return left * right;
        case '/':
            if (right == 0) throw std::runtime_error("div0");
            return left / right;
        default:
            throw std::runtime_error("badop");
    }
}

int RPN::evaluate(const std::string& expr) const {
    std::istringstream iss(expr);
    std::stack<long long> st;
    std::string tok;

    while (iss >> tok) {
        if (tok.size() == 1 && std::isdigit(static_cast<unsigned char>(tok[0]))) {
            st.push(tok[0] - '0');
        }
        else if (tok.size() == 1 && isOp(tok[0])) {
            if (st.size() < 2)
                throw std::runtime_error("stack");

            long long right = st.top(); st.pop();
            long long left  = st.top(); st.pop();

            long long res = applyOp(left, right, tok[0]);
            st.push(res);
        }
        else {
            throw std::runtime_error("token");
        }
    }

    if (st.size() != 1)
        throw std::runtime_error("final");

    return static_cast<int>(st.top());
}
