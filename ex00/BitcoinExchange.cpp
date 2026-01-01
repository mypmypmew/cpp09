#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <map>
#include <iterator>

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

bool BitcoinExchange::rateForDate(const std::string& date, double& outRate) const {

    if (_rates.empty()) {
        return false;
    }

    std::map<std::string, double>::const_iterator it = _rates.lower_bound(date);

    if (it == _rates.end()) {
        it = std::prev(_rates.end());
        outRate = it->second;
        return true;
    }

    if (it != _rates.end() && it->first == date) {
        outRate = it->second;
        return true;
    }  
    if (it == _rates.begin()) {
        return false;
    }
    else {
        --it;
        outRate = it->second;
        return true;
    }

    return false;
}

void BitcoinExchange::processInputFile(const std::string& filename) {

    std::ifstream file(filename);

    if(!file.is_open()) {
        std::cerr << "Error: could not open file.\n";
        return ;
    }

    std::string line;

        if (!std::getline(file, line)) {
        std::cerr << "Error: empty text file.\n";
        return ;
    }

    while (std::getline(file, line)) {
        if (trim(line).empty())
            continue;

        size_t pipePos = line.find('|');
        if (pipePos == std::string::npos) {
            std::cerr << "Error: bad input => " << line << "\n";

            continue;
        }

        const std::string inputDate = trim(line.substr(0, pipePos));
        const std::string inputAmount = trim(line.substr(pipePos + 1));

        if (inputDate.empty() || inputAmount.empty()) {
            std::cerr << "Error: bad input => " << line << "\n";

            continue;
        }

        /* Date validation? */

        double amount;
        if (!toDoubleStrict(inputAmount, amount)) {
            std::cerr << "Error: bad input => " << line << "\n";
            continue;
        }

        if (amount < 0) {
            std::cerr << "Error: not a positive number.\n";
            continue;
        }
        if (amount > 1000) {
            std::cerr << "Error: too large a number.\n";
            continue;
        }


        double rate;
        if (!rateForDate(inputDate, rate)) {
            std::cerr << "Error: bad input => " << line << "\n";
            continue;
        }
        double result = amount * rate;
        std::cout << inputDate << " => " << amount << " = " << result << "\n";
    }
}
