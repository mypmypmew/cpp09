#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <limits>
#include <chrono>
#include <algorithm>


template <typename Container>
static void pasteNumber(int x, Container& c) {
    auto pos = c.lower_bound(c.begin(), c.end(), x);
    c.insert(pos, x);
}


static bool parseInput(std::vector<int>& out, int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Error\n";
        return false;
    }

    out.reserve(static_cast<size_t>(argc - 1));

    for (int i = 1; i < argc; ++i) {
        try {
            std::size_t pos = 0;
            long long val = std::stoll(argv[i], &pos, 10);

            if (pos != std::string(argv[i]).size()) {
                std::cerr << "Error\n";
                return false;
            }

            if (val <= 0 || val > std::numeric_limits<int>::max()) {
                std::cerr << "Error\n";
                return false;
            }

            out.push_back(static_cast<int>(val));
        } catch (...) {
            std::cerr << "Error\n";
            return false;
        }
    }
    return true;
}

template <typename Container>
static void printSequence(const std::string& label, const Container& c, std::size_t maxPrint = 20) {
    std::cout << label;
    if (c.empty()) {
        std::cout << "\n";
        return;
    }

    std::size_t n = c.size();
    std::size_t limit = (n > maxPrint) ? maxPrint : n;

    for (std::size_t i = 0; i < limit; ++i) {
        std::cout << c[i];
        if (i + 1 < limit)
            std::cout << " ";
    }
    if (n > maxPrint)
        std::cout << " [...]";
    std::cout << "\n";
}

static void sortVector(std::vector<int>& v) {
    std::sort(v.begin(), v.end());
}

static void sortDeque(std::deque<int>& d) {
    std::sort(d.begin(), d.end());
}

int main(int argc, char** argv) {
    std::vector<int> input;
    std::vector<std::pair<int, int>> pairVector;

    if (!parseInput(input, argc, argv))
        return 1;

    printSequence("Before: ", input);

    for (size_t i = 0; i+1 < input.size(); i+=2) {
        int x = input[i];
        int y = input[i+1];
        if (x < y)
            pairVector.push_back({x, y});
        else 
            pairVector.push_back({y, x});
    }
    int left = -1;
    if (input.size() % 2 == 1)
        left = input[input.size() - 1];

    std::vector<int> v = input;
    std::deque<int>  d(input.begin(), input.end());

    auto t1 = std::chrono::high_resolution_clock::now();
    sortVector(v);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto vec_us = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    auto t3 = std::chrono::high_resolution_clock::now();
    sortDeque(d);
    auto t4 = std::chrono::high_resolution_clock::now();
    auto deq_us = std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();

    printSequence("After:  ", v);

    std::cout << "Time to process a range of " << v.size()
              << " elements with std::vector : " << vec_us << " us\n";
    std::cout << "Time to process a range of " << d.size()
              << " elements with std::deque  : " << deq_us << " us\n";

    return 0;
}
