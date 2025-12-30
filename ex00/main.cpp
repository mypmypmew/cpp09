#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <map>

int main(int argc, char **argv) {

    (void)argc;
    (void)argv;

    BitcoinExchange btc;

    btc.loadDatabase("data.csv");

    return 0;
}