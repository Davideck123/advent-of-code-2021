#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>

using RuleMap = std::map<std::string, char>;
using PairCounts = std::map<std::string, long long>;
using CharCounts = std::map<char, long long>;

void polymerization(const RuleMap& rules, PairCounts& pairCounts, CharCounts& charCounts, int steps) {
    for (int i = 0; i < steps; ++i) {
        PairCounts pairCountsNew;
        for (auto&& [key, count]: pairCounts) {
            char c = rules.at(key);
            std::string k1{ key[0], c }, k2{ c, key[1] };
            pairCountsNew[k1] += count;
            pairCountsNew[k2] += count;
            charCounts[c] += count;
        }
        pairCounts = pairCountsNew;
    }
}

void printDifference(const CharCounts& charCounts) {
    auto cmp = [] (const auto& a, const auto& b) { return a.second < b.second; };
    auto min = (*std::min_element(charCounts.cbegin(), charCounts.cend(), cmp)).second;
    auto max = (*std::max_element(charCounts.cbegin(), charCounts.cend(), cmp)).second;
    //for (auto&& x: charCounts) std::cout << x.first << ": " << x.second << std::endl;
    std::cout << max - min << std::endl;
}

int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);

    RuleMap rules;
    PairCounts pairCounts;
    CharCounts charCounts;

    char c = (char) dataFile.get();
    ++charCounts[c];
    while (!std::isspace(dataFile.peek())) {
        ++pairCounts[std::string{ c, (char) dataFile.peek() }];
        c = (char) dataFile.get();
        ++charCounts[c];
    }
    // skip newlines
    dataFile.get();
    dataFile.get();

    while (dataFile.peek() != EOF) {
        std::string s{ (char) dataFile.get(), (char) dataFile.get() };
        while (!std::isalpha(dataFile.peek())) dataFile.get();
        rules.emplace(s, dataFile.get());
        dataFile.get(); // skip newline
    }
    dataFile.close();

    polymerization(rules, pairCounts, charCounts, 10);
    printDifference(charCounts);
    polymerization(rules, pairCounts, charCounts, 30);
    printDifference(charCounts);
}