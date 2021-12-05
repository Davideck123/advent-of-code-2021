#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <vector>

struct MyHash {
    size_t operator()(const std::pair<int, int>& k) const {
        auto hash1 = std::hash<int>{}(k.first);
        auto hash2 = std::hash<int>{}(k.second);
        return hash1 ^ hash2;
    }
};

using CoordDict = std::unordered_map<std::pair<int, int>, int, MyHash>;

bool isHorizontal(int y1, int y2) {
    return y1 == y2;
}

bool isVertical(int x1, int x2) {
    return x1 == x2;
}

void solve(std::istream& in, bool diagonal) {
    CoordDict cd;
    std::string line;
    while (std::getline(in, line)) {
        // append space to line to read numbers easier
        line += ' ';
        std::string num;
        std::vector<int> coords;
        // coords[0] = x1, coords[1] = y1, coords[2] = x2, coords[3] = y2
        for (int i = 0; i < line.size(); ++i) {
            if (std::isdigit(line[i])) num += line[i];
            else if (!num.empty()) {
                coords.push_back(std::stoi(num));
                num.clear();
            }
        }

        if (isHorizontal(coords[1], coords[3])) {
            for (int x = std::min(coords[0], coords[2]); x <= std::max(coords[0], coords[2]); ++x) {
                std::pair newCoords(x, coords[1]);
                if (cd.find(newCoords) != cd.end()) ++cd[newCoords];
                else cd.emplace(newCoords, 1);
            }
        }
        else if (isVertical(coords[0], coords[2])) {
            for (int y = std::min(coords[1], coords[3]); y <= std::max(coords[1], coords[3]); ++y) {
                std::pair newCoords(coords[0], y);
                if (cd.find(newCoords) != cd.end()) ++cd[newCoords];
                else cd.emplace(newCoords, 1);
            }
        }
        else if (diagonal) {
            if (coords[2] < coords[0]) {
                int x = coords[2];
                int y = coords[3];
                while (x <= coords[0]) {
                    std::pair newCoords(x, y);
                    if (cd.find(newCoords) != cd.end()) ++cd[newCoords];
                    else cd.emplace(newCoords, 1);
                    ++x;
                    y = (coords[3] < coords[1]) ? y + 1 : y - 1;
                }
            }
            else {
                int x = coords[0];
                int y = coords[1];
                while (x <= coords[2]) {
                    std::pair newCoords(x, y);
                    if (cd.find(newCoords) != cd.end()) ++cd[newCoords];
                    else cd.emplace(newCoords, 1);
                    ++x;
                    y = (coords[1] < coords[3]) ? y + 1 : y - 1;
                }
            }
        }
    }

    int count = 0;
    for (auto&& x: cd) {
        if (x.second > 1) ++count;
    }
    std::cout << count << std::endl;
}

void partOne(std::istream& in) {
    solve(in, false);
}

void partTwo(std::istream& in) {
    solve(in, true);
}

int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);
    partOne(dataFile);

    // return to the begging of the file
    dataFile.clear();
    dataFile.seekg(0);

    partTwo(dataFile);
    dataFile.close();
}