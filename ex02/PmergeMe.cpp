#include "PmergeMe.hpp"

#include <iostream>
#include <limits>
#include <chrono>
#include <algorithm>
#include <set>

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe& other) : _input(other._input) {}
PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other)
        _input = other._input;
    return *this;
}
PmergeMe::~PmergeMe() {}

static size_t jacobsthal(size_t k) {
    if (k == 0) return 0;
    if (k == 1) return 1;
    size_t a = 0, b = 1;
    for (size_t i = 2; i <= k; ++i) {
        size_t c = b + 2 * a;
        a = b;
        b = c;
    }
    return b;
}

static std::vector<size_t> buildJacobOrder(size_t pendSize) {
    std::vector<size_t> order;
    if (pendSize <= 1) return order;

    size_t prev = 1;
    for (size_t k = 3;; ++k) {
        size_t j = jacobsthal(k);
        if (j > pendSize) break;

        for (size_t idx = j; idx > prev; --idx)
            order.push_back(idx - 1);

        prev = j;
    }

    for (size_t idx = pendSize; idx > prev; --idx)
        order.push_back(idx - 1);

    return order;
}


static void insertNumberVector(std::vector<int>& mainChain, int small, int big) {

    auto iterBig = std::lower_bound(mainChain.begin(), mainChain.end(), big);
    auto pos = std::lower_bound(mainChain.begin(), iterBig, small);
    mainChain.insert(pos, small);
}

static void insertNumberDeque(std::deque<int>& mainChain, int small, int big) {
    auto iterBig = std::lower_bound(mainChain.begin(), mainChain.end(), big);
    auto pos = std::lower_bound(mainChain.begin(), iterBig, small);
    mainChain.insert(pos, small);
}

void PmergeMe::FordJohnsonSortDeque(std::deque<int>& d) {
    if (d.size() <= 1) return;

    std::deque<int> mainChain;
    std::vector<std::pair<int,int>> leftPairs;

    int left = -1;
    bool hasLeft = (d.size() % 2 == 1);
    if (hasLeft) left = d.back();

    for (size_t i = 0; i + 1 < d.size(); i += 2) {
        int a = d[i], b = d[i + 1];
        int small = (a < b) ? a : b;
        int big   = (a < b) ? b : a;
        leftPairs.push_back({small, big});
        mainChain.push_back(big);
    }

    FordJohnsonSortDeque(mainChain);

    std::vector<std::pair<int, int>> leftPairsOrder;
    leftPairsOrder.reserve(leftPairs.size());
    std::vector<bool> used(leftPairs.size(), false);

    for (int big : mainChain) {
        for (size_t i = 0; i < leftPairs.size(); ++i) {
            if (!used[i] && leftPairs[i].second == big) {
                used[i] = true;
                leftPairsOrder.push_back(leftPairs[i]);
                break;
            }
        }
    }
    leftPairs.swap(leftPairsOrder);

    if (!leftPairs.empty()) {
        insertNumberDeque(mainChain, leftPairs[0].first, leftPairs[0].second);

        std::vector<size_t> order = buildJacobOrder(leftPairs.size());
        for (size_t idx : order) {
            insertNumberDeque(mainChain, leftPairs[idx].first, leftPairs[idx].second);
        }
    }

    if (hasLeft) {
        auto pos = std::lower_bound(mainChain.begin(), mainChain.end(), left);
        mainChain.insert(pos, left);
    }

    d.swap(mainChain);
}

void PmergeMe::FordJohnsonSortVector(std::vector<int>& v) {
    if (v.size() <= 1) {
        return ;
    }

    std::vector<int> mainChain;
    std::vector<std::pair<int,int>> leftPairs;

    int left = -1;
    bool hasLeft = (v.size() % 2 == 1);

    if (hasLeft) {
        left = v.back();
    }

    for (size_t i = 0; i + 1 < v.size(); i += 2) {
        int a = v[i], b = v[i + 1];
        int small = (a < b) ? a : b;
        int big   = (a < b) ? b : a;
        leftPairs.push_back({small, big});
        mainChain.push_back(big);
    }
    FordJohnsonSortVector(mainChain);

    std::vector<std::pair<int, int>> leftPairsOrder;
    leftPairsOrder.reserve(leftPairs.size());
    std::vector<bool> used(leftPairs.size(), false);

        for (int big : mainChain) {
        for (size_t i = 0; i < leftPairs.size(); ++i) {
            if (!used[i] && leftPairs[i].second == big) {
                used[i] = true;
                leftPairsOrder.push_back(leftPairs[i]);
                break;
            }
        }
    }
    leftPairs.swap(leftPairsOrder);

    if (!leftPairs.empty()) {
        insertNumberVector(mainChain, leftPairs[0].first, leftPairs[0].second);

        std::vector<size_t> order = buildJacobOrder(leftPairs.size());

        for (size_t idx : order) {
            insertNumberVector(mainChain, leftPairs[idx].first, leftPairs[idx].second);
        }
    }

    if (hasLeft) {
        auto pos = std::lower_bound(mainChain.begin(), mainChain.end(), left);
        mainChain.insert(pos, left);
    }

    v.swap(mainChain);
}

bool PmergeMe::parseInput(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Error\n";
        return false;
    }

    _input.reserve(static_cast<size_t>(argc - 1));
    _input.clear();

    std::set<int> seen;

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

            int x = static_cast<int>(val);

            if (!seen.insert(x).second) {
                std::cerr << "Error\n";
                return false;
            }

            

            _input.push_back(x);
        } catch (...) {
            std::cerr << "Error\n";
            return false;
        }
    }
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

int PmergeMe::run(int argc, char** argv) {
    if (!parseInput(argc, argv))
        return 1;

    printBefore();

    std::vector<int> v = _input;
    std::deque<int>  d(_input.begin(), _input.end());

    //victor
    auto t1 = std::chrono::high_resolution_clock::now();
    FordJohnsonSortVector(v);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto vec_us = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    //deque
    auto t3 = std::chrono::high_resolution_clock::now();
    FordJohnsonSortDeque(d);
    auto t4 = std::chrono::high_resolution_clock::now();
    auto deq_us = std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();

    if (v.size() != d.size() || !std::equal(v.begin(), v.end(), d.begin())) {
        std::cerr << "Error\n";
        return 1;
    }
    
    printAfter(v);

    std::cout << "Time to process a range of " << v.size()
              << " elements with std::vector : " << vec_us << " us\n";
    std::cout << "Time to process a range of " << d.size()
              << " elements with std::deque  : " << deq_us << " us\n";


    return 0;
}