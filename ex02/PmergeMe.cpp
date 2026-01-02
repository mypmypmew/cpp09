#include "PmergeMe.hpp"

#include <iostream>
#include <limits>
#include <chrono>
#include <algorithm>
#include <unordered_set>
#include <cstdlib>

PmergeMe::PmergeMe() {};
PmergeMe::PmergeMe(const PmergeMe& other) : _input(other._input) {}
PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other)
        _input = other._input;
    return *this;
};
PmergeMe::~PmergeMe() {};



bool PmergeMe::parseInput(int argc, char** argv) {


    return true;
}

void PmergeMe::printSequence(const std::string& label,
                             const std::vector<int>& c,
                             std::size_t maxPrint) {
    std::cout << label;
    std::size_t n = c.size();
    std::size_t limit = (n > maxPrint) ? maxPrint : n;

    for (std::size_t i = 0; i < limit; ++i) {
        std::cout << c[i];
        if (i + 1 < limit) std::cout << " ";
    }
    if (n > maxPrint) std::cout << " [...]";
    std::cout << "\n";
}


void PmergeMe::printBefore() const {
    printSequence("Before: ", _input);
}

void PmergeMe::printAfter(const std::vector<int>& v) const {
    printSequence("After:  ", v);
}

static void sortVector(std::vector<int>& v) {
    std::sort(v.begin(), v.end());
}

static void sortDeque(std::deque<int>& d) {
    std::sort(d.begin(), d.end());
}

int PmergeMe::run(int argc, char** argv) {
    if (!parseInput(argc, argv))
        return 1;
        
    printBefore();

    std::vector<int> v = _input;
    std::deque<int>  d(_input.begin(), _input.end());
    
}