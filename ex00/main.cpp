#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <map>

static std::string trim(const std::string &s) {
    size_t start = 0;
    while (start < s.size() && (std::isspace(static_cast<unsigned char>(s[start])) || s[start] == '\r'))
        ++start;

    size_t end = s.size();
    while (end > start && (std::isspace(static_cast<unsigned char>(s[end - 1])) || s[end - 1] == '\r'))
        --end;

    return s.substr(start, end - start);
}

static bool toDoubleStrict(const std::string &s, double &out) {
    std::string t = trim(s);
    if (t.empty())
        return false;
    
    char *endptr = NULL;
    out = std::strtod(t.c_str(), &endptr);

    if (endptr == t.c_str() || *endptr != '\0')
        return false;
    return true;
}

int main(int argc, char **argv) {

    (void)argc;
    (void)argv;

    std::ifstream file("data.csv");

    if (!file.is_open()) {
        std::cerr << "Error: could not open file.";
        return 1;
    }

    std::map<std::string, double> rates;

    std::string line;

    if (!std::getline(file, line)) {
        std::cerr << "Empry CSV\n";
        return 1;
    }

    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        size_t comma = line.find(',');
        if (comma == std::string::npos) {
            std::cerr << "Bad CSV line: " << line << "\n";
        }

        std::string date = trim(line.substr(0, comma));
        std::string rateStr = trim(line.substr(comma + 1));

        double rate = 0.0;
        if (date.empty() || !toDoubleStrict(rateStr, rate)) {
            std::cerr << "Bad CSV data: " << line << "\n";
            continue;
        }

        rates[date] = rate;
    }

        std::cout << "Loaded " << rates.size() << " rates.\n";
        int shown = 0;
        for (std::map<std::string, double>::iterator it = rates.begin();
            it != rates.end() && shown < 5;
            ++it, ++shown) {
                std::cout << it->first << " -> " << it->second << "\n";
            }
    
    return 0;
}