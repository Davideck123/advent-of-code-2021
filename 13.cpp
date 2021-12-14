#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>

using Point = std::pair<int, int>;

void fold(char axis, int index, std::set<Point>& dotIndices) {
    for (auto it = dotIndices.begin(); it != dotIndices.end();) {
        if (axis == 'x' && (*it).first > index) {
            dotIndices.emplace(index - ((*it).first - index), (*it).second);
            it = dotIndices.erase(it);
        }
        else if (axis == 'y' && (*it).second > index) {
            dotIndices.emplace((*it).first, index - ((*it).second - index));
            it = dotIndices.erase(it);
        }
        else ++it;
    }
}

void printResult(const std::set<Point>& dotIndices) {
    int maxX = (*std::max_element(dotIndices.cbegin(), dotIndices.cend())).first;
    int maxY = (*std::max_element(dotIndices.cbegin(), dotIndices.cend(),
                [] (auto x1, auto x2) { return x1.second < x2.second; })).second;
    for (int i = 0; i <= maxY; ++i) {
        for (int j = 0; j <= maxX; ++j) {
            if (dotIndices.find({ j, i }) != dotIndices.cend()) std::cout << '#';
            else std::cout << '.';
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);
    std::set<Point> dotIndices;

    // read dot indices
    while (dataFile.peek() != '\n') {
        std::string s1, s2;
        std::getline(dataFile, s1, ',');
        std::getline(dataFile, s2);
        dotIndices.emplace(std::stoi(s1), std::stoi(s2));
    }
    dataFile.get();

    // read fold instructions
    bool firstFold = true;
    while (dataFile.good()) {
        char c = dataFile.get();
        if (c == 'x' || c == 'y') {
            dataFile.get();
            std::string s;
            std::getline(dataFile, s);
            fold(c, std::stoi(s), dotIndices);
            if (firstFold) {
                // partOne
                std::cout << dotIndices.size() << std::endl;
                firstFold = false;
            }
        }
    }
    dataFile.close();
    // partTwo
    printResult(dotIndices);
}