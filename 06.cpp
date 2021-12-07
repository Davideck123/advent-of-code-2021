#include <iostream>
#include <fstream>
#include <string>
#include <array>

using FishMap = std::array<long long, 9>;

void solve(FishMap& fish, int days) {
    for (int i = 0; i < days; ++i) {
        long long newFish = 0;
        for (int j = 0; j < fish.size(); ++j) {
            if (j == 0) newFish = fish[j];
            else fish[j - 1] = fish[j];
            fish[j] = 0;
        }
        fish[6] += newFish;
        fish[8] = newFish;
    }
    long long sum = 0;
    for (auto&& x: fish) sum += x;
    std::cout << sum << std::endl;
}


int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);

    FishMap fish{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    std::string num;
    while (std::getline(dataFile, num, ',')) {
        ++fish[std::stoi(num)];
    }
    dataFile.close();

    // partOne
    //solve(fish, 80);
    // partTwo
    solve(fish, 256);
}