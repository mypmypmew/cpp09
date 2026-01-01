#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <map>
#include <iterator>

BitcoinExchange::BitcoinExchange() {};
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) 
            : _rates(other._rates) {};
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
        if (this != &other) {
        _rates = other._rates;
    }
    return *this;
};
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
        std::cerr << "Error: could not open file.\n";
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

static bool isValidDateFormat(const std::string& s) {
    if (s.size() != 10)
        return false;
    if (s[4] != '-' || s[7] != '-')
        return false;

    for (size_t i = 0; i < s.size(); ++i) {
        if (i == 4 || i == 7)
            continue;
        if (!std::isdigit(static_cast<unsigned char>(s[i])))
            return false;
    }
    return true;
}

static int toInt2(const std::string& s, size_t pos) {
    return (s[pos] - '0') * 10 + (s[pos + 1] - '0');
}

static int toInt4(const std::string& s, size_t pos) {
    return (s[pos] - '0') * 1000
         + (s[pos + 1] - '0') * 100
         + (s[pos + 2] - '0') * 10
         + (s[pos + 3] - '0');
}

static bool isLeapYear(int y) {
    if (y % 400 == 0) return true;
    if (y % 100 == 0) return false;
    return (y % 4 == 0);
}

static int daysInMonth(int y, int m) {
    static const int days[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    if (m == 2) return days[1] + (isLeapYear(y) ? 1 : 0);
    return days[m - 1];
}

static bool isValidDate(const std::string& s) {
    if (!isValidDateFormat(s))
        return false;

    int y = toInt4(s, 0);
    int m = toInt2(s, 5);
    int d = toInt2(s, 8);

    if (m < 1 || m > 12) return false;
    int dim = daysInMonth(y, m);
    if (d < 1 || d > dim) return false;

    return true;
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

    if (it->first == date) {
        outRate = it->second;
        return true;
    }  
    if (it == _rates.begin()) {
        return false;
    }
    --it;
    outRate = it->second;
    return true;
}

void BitcoinExchange::processInputFile(const std::string& filename) {

    std::ifstream file(filename);

    if(!file.is_open()) {
        std::cerr << "Error: could not open file.\n";
        return ;
    }

    std::string line;

        if (!std::getline(file, line)) {
        std::cerr << "Error: bad input => " << line << "\n";
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

        if (!isValidDate(inputDate)) {
            std::cerr << "Error: bad input => " << line << "\n";
            continue;
        }

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
