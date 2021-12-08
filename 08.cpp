#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>

using NumberMap = std::map<int, std::string>;

bool isOne(const std::string& s) {
    return s.size() == 2;
}

bool isSeven(const std::string& s) {
    return s.size() == 3;
}

bool isFour(const std::string& s) {
    return s.size() == 4;
}

bool isEight(const std::string& s) {
    return s.size() == 7;
}

std::string setDifference(const std::string& s1, const std::string& s2) {
    std::string s;
    for (auto&& x: s1) {
        // if s2 doesn't contain x
        if (s2.find(x) == std::string::npos) s += x;
    }
    return s;
}

bool containsNum(const std::string& src, const std::string& num) {
    for (auto&& x: num) {
        if (src.find(x) == std::string::npos) return false;
    }
    return true;
}

int getKey(const std::string& value, const NumberMap& map) {
    for (auto&& x: map) {
        if (containsNum(x.second, value) && x.second.size() == value.size()) {
            return x.first;
        }
    }
    return -1;
}

int solve(const std::string& line) {
    std::stringstream ss{ line };
    std::string s;
    NumberMap map;
    std::vector<std::string> fiveCharsLong, sixCharsLong;

    // read numbers
    for (int i = 0; i < 10; ++i) {
        ss >> s;
        if (isOne(s)) map[1] = s;
        else if (isSeven(s)) map[7] = s;
        else if (isFour(s)) map[4] = s;
        else if (isEight(s)) map[8] = s;
        else if (s.size() == 5) fiveCharsLong.emplace_back(s);
        else sixCharsLong.emplace_back(s);
    }

    // get three
    for (int i = 0; i < fiveCharsLong.size(); ++i) {
        if (containsNum(fiveCharsLong[i], map[1])) {
            map[3] = fiveCharsLong[i];
            fiveCharsLong.erase(fiveCharsLong.begin() + i);
            break;
        }
    }

    // get nine
    for (int i = 0; i < sixCharsLong.size(); ++i) {
        if (containsNum(sixCharsLong[i], map[3])) {
            map[9] = sixCharsLong[i];
            sixCharsLong.erase(sixCharsLong.begin() + i);
            break;
        }
    }

    for (auto&& x: sixCharsLong) {
        if (containsNum(x, map[1])) map[0] = x;
        else map[6] = x;
    }

    auto res = setDifference(map[8], map[6]);
    for (auto&& x: fiveCharsLong) {
        if (containsNum(x, res)) map[2] = x;
        else map[5] = x;
    }

    // delimiter we can skip
    ss >> s;
    int num = 0;
    for (int i = 0; i < 4; ++i) {
        ss >> s;
        num = 10 * num + getKey(s, map);
    }
    return num;
}

void partTwo(std::istream& in) {
    std::string line;
    int sum = 0;
    while (std::getline(in, line)) {
        sum += solve(line);
    }
    std::cout << sum << std::endl;
}

void partOne(std::istream& in) {
    int count = 0;
    std::string s;
    while (std::getline(in, s, '|')) {
        std::getline(in, s);
        std::stringstream ss{ s };
        for (int i = 0; i < 4; ++i) {
            ss >> s;
            if (isOne(s) || isSeven(s) || isFour(s) || isEight(s)) ++count;
        }
    }
    std::cout << count << std::endl;
}

int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);

    //partOne(dataFile);
    partTwo(dataFile);
    dataFile.close();
}