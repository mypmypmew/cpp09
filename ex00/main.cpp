#include "BitcoinExchange.hpp"
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Error: could not open file.\n";
        return 1;
    }

    BitcoinExchange btc;

    btc.loadDatabase("data.csv");

    btc.processInputFile(argv[1]);

    return 0;
}