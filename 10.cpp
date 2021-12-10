#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <map>
#include <vector>
#include <algorithm>

struct BracketChecker {
    bool isOpen(char c) {
        if (c == '(' || c == '[' || c == '{' || c == '<') return true;
        return false;
    }

    std::map<char, char> matchingBracket_{ { ')', '(' },
                                           { ']', '[' },
                                           { '}', '{' },
                                           { '>', '<' } };
    std::map<char, int> illegalValue_{ { ')', 3 },
                                       { ']', 57 },
                                       { '}', 1197 },
                                       { '>', 25137 } };
    std::map<char, int> completionValue_{ { '(', 1 },
                                          { '[', 2 },
                                          { '{', 3 },
                                          { '<', 4 } };
};

int checkLine(const std::string& line) {
    BracketChecker bc;
    std::stack<char> brackets;
    for (auto&& x: line) {
        if (bc.isOpen(x)) brackets.push(x);
        else if (brackets.top() != bc.matchingBracket_[x]) return bc.illegalValue_[x];
        else brackets.pop();
    }
    return 0;
}

long long completeLine(const std::string& line) {
    BracketChecker bc;
    std::stack<char> brackets;
    for (auto&& x: line) {
        if (bc.isOpen(x)) brackets.push(x);
        else if (brackets.top() != bc.matchingBracket_[x]) return -1;
        else brackets.pop();
    }
    long long completionSum = 0;
    while (!brackets.empty()) {
         completionSum = 5 * completionSum + bc.completionValue_[brackets.top()];
         brackets.pop();
    }
    return completionSum;
}

void partOne(std::istream& in) {
    std::string line;
    int illegalSum = 0;
    while (std::getline(in, line)) {
        illegalSum += checkLine(line);
    }
    std::cout << illegalSum << std::endl;
}

void partTwo(std::istream& in) {
    std::string line;
    std::vector<long long> completionSums;
    while (std::getline(in, line)) {
        long long result = completeLine(line);
        if (result != -1) completionSums.push_back(result);
    }
    std::sort(completionSums.begin(), completionSums.end());
    std::cout << completionSums[completionSums.size() / 2] << std::endl;
}

int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);

    //partOne(dataFile);
    partTwo(dataFile);
    dataFile.close();
}