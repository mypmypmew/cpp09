#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <string>
#include <vector>
#include <deque>

class PmergeMe {
    public:
        PmergeMe();
        PmergeMe(const PmergeMe& other);
        PmergeMe& operator=(const PmergeMe& other);
        ~PmergeMe();

        int run(int argc, char** argv);

    private:
        std::vector<int> _input;

        void printBefore() const;
        void printAfter(const std::vector<int>& v) const;
        bool parseInput(int argc, char** argv);
        void FordJohnsonSortVector(std::vector<int>& v);
        void FordJohnsonSortDeque(std::deque<int>& d);

        static void printSequence(const std::string& label,
                                const std::vector<int>& c,
                                std::size_t maxPrint = 20);

};

#endif