#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <map>

BitcoinExchange::BitcoinExchange() {};
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) {};
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {};
BitcoinExchange::~BitcoinExchange() {};

static std::string trim(std::string s) {
    auto is_space = [](unsigned char c) {
        return std::isspace(c) || c == '\r';
    };

    while (!s.empty() && is_space((unsigned char)s.front())) s.erase(s.begin());
    while (!s.empty() && is_space((unsigned char)s.back()))  s.pop_back();
    return s;
}

static bool toDoubleStrict(const std::string& s, double& out) {
    std::string t = trim(s);
    if (t.empty()) return false;

    try {
        size_t pos = 0;
        out = std::stod(t, &pos);
        return pos == t.size();
    } catch (...) {
        return false;
    }
}

void BitcoinExchange::loadDatabase(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: could not open file.";
        return ;
    }

    std::string line;

    if (!std::getline(file, line)) {
        std::cerr << "Error: empty database.\n";
        return ;
    }

    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        size_t comma = line.find(',');
        if (comma == std::string::npos) {
            // std::cerr << "Bad CSV line: " << line << "\n";
            continue;
        }

        const std::string date = trim(line.substr(0, comma));
        const std::string rateStr = trim(line.substr(comma + 1));

        double rate = 0.0;
        if (date.empty() || !toDoubleStrict(rateStr, rate)) {
            // std::cerr << "Bad CSV data: " << line << "\n";
            continue;
        }

        _rates[date] = rate;
    }

}
