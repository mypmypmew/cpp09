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
};

#endif