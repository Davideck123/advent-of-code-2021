#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <queue>
#include <algorithm>

using HeightMap = std::array<std::array<int, 100>, 100>;

int charToInt(int c) {
    return c - '0';
}

bool isLowest(const HeightMap& heightMap, int x, int y) {
    int height = heightMap[x][y];
    if (x > 0 && heightMap[x - 1][y] <= height) return false;
    if (y > 0 && heightMap[x][y - 1] <= height) return false;
    if (y < heightMap[0].size() - 1 && heightMap[x][y + 1] <= height) return false;
    if (x < heightMap.size() - 1 && heightMap[x + 1][y] <= height) return false;
    return true;
}

void partOne(const HeightMap& heightMap) {
    int riskLevel = 0;
    for (int i = 0; i < heightMap.size(); ++i) {
        for (int j = 0; j < heightMap[i].size(); ++j) {
            if (isLowest(heightMap, i, j)) riskLevel += heightMap[i][j] + 1;
        }
    }
    std::cout << riskLevel << std::endl;
}

int searchBasin(HeightMap& heightMap, int row, int column) {
    // BFS search
    std::queue<std::pair<int, int>> queue;
    queue.emplace(row, column);
    int size = 0;
    while (!queue.empty()) {
        auto point = queue.front();
        queue.pop();
        int i = point.first;
        int j = point.second;
        if (heightMap[i][j] != 9) {

            if (i > 0 && heightMap[i - 1][j] != 9) queue.emplace(i - 1, j);
            if (j > 0 && heightMap[i][j - 1] != 9) queue.emplace(i, j - 1);
            if (j < heightMap[0].size() - 1 && heightMap[i][j + 1] != 9) queue.emplace(i, j + 1);
            if (i < heightMap.size() - 1 && heightMap[i + 1][j] != 9) queue.emplace(i + 1, j);

            ++size;
            heightMap[i][j] = 9;
        }
    }
    return size;
}

void partTwo(HeightMap& heightMap) {
    std::vector<int> largestBasins;
    for (int i = 0; i < heightMap.size(); ++i) {
        for (int j = 0; j < heightMap[i].size(); ++j) {
            if (heightMap[i][j] != 9) {
                largestBasins.push_back(searchBasin(heightMap, i, j));
                if (largestBasins.size() > 3)
                    largestBasins.erase(std::min_element(largestBasins.begin(), largestBasins.end()));
            }
        }
    }
    std::cout << largestBasins[0] * largestBasins[1] * largestBasins[2] << std::endl;
}

int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);
    HeightMap heightMap;

    for (int i = 0; i < heightMap.size(); ++i) {
        for (int j = 0; j < heightMap[i].size(); ++j) {
            heightMap[i][j] = charToInt(dataFile.get());
        }
        // get rid of newline
        dataFile.get();
    }
    dataFile.close();

    partOne(heightMap);
    partTwo(heightMap);
}