#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <limits>
#include <chrono>
#include <algorithm>
#include "PmergeMe.hpp"

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

void FordJohnsonSortVector(std::vector<int>& v) {
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
    PmergeMe app;
    return app.run(argc, argv);
}
