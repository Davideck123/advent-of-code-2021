#include <iostream>
#include <fstream>
#include <queue>
#include <deque>
#include <vector>

using Cavern = std::vector<std::vector<int>>;
using Point = std::pair<int, int>;

#define SIZE 100

int charToInt(int c) {
    return c - '0';
}

Cavern readInput(std::ifstream& in) {
    Cavern risks{ SIZE, std::vector<int>(SIZE) };
    for (auto&& row: risks) {
        for (auto&& col: row) {
            col = charToInt(in.get());
        }
        // skip newline
        in.get();
    }
    return risks;
}

bool isValid(int row, int col, int size = SIZE) {
    return 0 <= row && row < size && 0 <= col && col < size;
}

void search(const Cavern& risks) {
    // Dijkstra
    auto cmp = [] (const auto& a, const auto& b) { return a.second > b.second; };
    std::priority_queue<std::pair<Point, int>,
                        std::deque<std::pair<Point, int>>,
                        decltype(cmp)> queue{ cmp };
    const int UNDISCOVERED = -1;
    Cavern discovered{ risks.size(), std::vector<int>(risks.size(), UNDISCOVERED) };

    queue.push({{ 0, 0 }, 0 });
    const std::vector<Point> ADJACENT{ std::make_pair(-1, 0),
                                       std::make_pair(0, -1),
                                       std::make_pair(0, 1),
                                       std::make_pair(1, 0) };

    while (!queue.empty() && discovered[discovered.size() - 1][discovered.size() - 1] == UNDISCOVERED) {
        auto row = queue.top().first.first;
        auto col = queue.top().first.second;
        auto len = queue.top().second;
        if (discovered[row][col] == UNDISCOVERED) {
            for (auto&& [i, j]: ADJACENT) {
                if (isValid(row + i, col + j, (int) risks.size()) && discovered[row + i][col + j] == UNDISCOVERED) {
                    queue.push({ { row + i, col + j }, len + risks[row + i][col + j] });
                }
            }
            discovered[row][col] = len;
        }
        queue.pop();
    }

    std::cout << "Length: " << discovered[discovered.size() - 1][discovered.size() - 1] << std::endl;
}

int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);
    Cavern risks = readInput(dataFile);
    dataFile.close();

    // partOne
    search(risks);

    Cavern risksLarge{ risks.size() * 5, std::vector<int>(risks.size() * 5) };
    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            for (int i = 0; i < risks.size(); ++i) {
                for (int j = 0; j < risks.size(); ++j) {
                    auto row = x * risks.size() + i;
                    auto col = y * risks.size() + j;
                    auto newVal = risks[i][j] + x * 1 + y * 1;
                    risksLarge[row][col] = (newVal > 9) ? newVal % 10 + 1 : newVal;
                }
            }
        }
    }
    // partTwo
    search(risksLarge);
}