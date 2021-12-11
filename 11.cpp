#include <iostream>
#include <fstream>
#include <array>
#include <queue>

#define STEPS 100

using OctopusTable = std::array<std::array<int , 10>, 10>;
using Point = std::pair<int, int>;

int charToInt(int c) {
    return c - '0';
}

void addAdjacent(const OctopusTable& table, std::queue<Point>& queue, int i, int j) {
    if (i > 0 && j > 0) queue.emplace(i - 1, j - 1);
    if (i > 0) queue.emplace(i - 1, j);
    if (i > 0 && j < table[i].size() - 1) queue.emplace(i - 1, j + 1);
    if (j > 0) queue.emplace(i, j - 1);
    if (j < table[i].size() - 1) queue.emplace(i, j + 1);
    if (i < table.size() - 1 && j > 0) queue.emplace(i + 1, j - 1);
    if (i < table.size() - 1) queue.emplace(i + 1, j);
    if (i < table.size() - 1 && j < table[i].size() - 1) queue.emplace(i + 1, j + 1);
}

int step(OctopusTable& table) {
    std::queue<Point> queue;
    int flashes = 0;
    for (int i = 0; i < table.size(); ++i) {
        for (int j = 0; j < table[i].size(); ++j) {
            if (++table[i][j] > 9) {
                addAdjacent(table, queue, i, j);
                ++flashes;
                table[i][j] = 0;
            }
        }
    }
    while (!queue.empty()) {
        int i = queue.front().first;
        int j = queue.front().second;
        queue.pop();
        // if table[i][j] == 0, it already flashed in this step
        if (table[i][j] > 0 && ++table[i][j] > 9) {
            addAdjacent(table, queue, i, j);
            ++flashes;
            table[i][j] = 0;
        }
    }

    return flashes;
}

void partOne(OctopusTable& table) {
    int flashCount = 0;
    for (int x = 0; x < STEPS; ++x) {
        flashCount += step(table);
    }
    std::cout << flashCount << std::endl;
}

bool allFlashed(const OctopusTable& table) {
    for (auto&& x: table) {
        for (auto&& y: x) {
            if (y != 0) return false;
        }
    }
    return true;
}

void partTwo(OctopusTable& table) {
    int steps = 0;
    while (!allFlashed(table)) {
        step(table);
        ++steps;
    }
    std::cout << steps << std::endl;
}

int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);
    OctopusTable table;

    for (auto&& x: table) {
        for (auto&& y: x) {
            y = charToInt(dataFile.get());
        }
        // get rid of newline
        dataFile.get();
    }
    dataFile.close();

    //partOne(table);
    partTwo(table);
}