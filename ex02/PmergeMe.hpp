#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <stack>
#include <string>

class PmergeMe {
    public:
        PmergeMe();
        PmergeMe(const PmergeMe& other);
        PmergeMe& operator=(const PmergeMe& other);
        ~PmergeMe();

        int run(int argc, char** argv);

    private:
        std::vector<int>_input;

        bool parseInput(int argc, char**argv);
        void printBefore() const;
        void printAfter(const std::vector<int>& v) const;

        static void printSequence(const std::string& label,
                                const std::vector<int>& c,
                                std::size_t maxPrint = 20);

};

#endif