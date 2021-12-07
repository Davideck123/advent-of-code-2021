#include <iostream>
#include <fstream>
#include <string>
#include <map>

using FishMap = std::map<int, long long>;

void solve(FishMap& fish, int days) {
    for (int i = 0; i < days; ++i) {
        long long newFish = 0;
        for (auto it = fish.begin(); it != fish.end(); ++it) {
            if (it == fish.begin()) newFish = it->second;
            else fish[it->first - 1] = it->second;
            it->second = 0;
        }
        fish[6] += newFish;
        fish[8] = newFish;
    }
    long long sum = 0;
    for (auto&& x: fish) sum += x.second;
    std::cout << sum << std::endl;
}


int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);

    FishMap fish;
    for (int i = 0; i < 9; ++i) {
        fish.emplace(i, 0);
    }
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