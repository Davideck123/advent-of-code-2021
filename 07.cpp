#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

void partOne(std::vector<int>& crabs) {
    std::sort(crabs.begin(), crabs.end());
    int median = (crabs[(crabs.size() - 1) / 2] + crabs[(crabs.size() + 1) / 2]) / 2;
    int fuel = 0;
    for (auto&& x: crabs) fuel += abs(x - median);
    std::cout << fuel << std::endl;
}

int arithmeticSequenceSum(int n, int d = 1, int first = 1) {
    int nth = first + (n - 1) * d;
    return n * (first + nth) / 2;
}

void partTwoBruteForce(const std::vector<int>& crabs) {
    int min = *std::min_element(crabs.cbegin(), crabs.cend());
    int max = *std::max_element(crabs.cbegin(), crabs.cend());
    int minFuel = INT_MAX;
    int minIndex = -1;
    for (int i = min; i <= max ; ++i) {
        int fuel = 0;
        for (auto&&x: crabs) fuel += arithmeticSequenceSum(abs(x - i));
        if (fuel < minFuel) {
            minFuel = fuel;
            minIndex = i;
        }
    }
    std::cout << "Index: " << minIndex << " Fuel: " << minFuel << std::endl;
}

void partTwo(const std::vector<int>& crabs) {
    int sum = std::accumulate(crabs.cbegin(), crabs.cend(), 0);
    double mean = sum / (double) crabs.size();
    int estimate1 = static_cast<int>(round(mean - 0.5));
    int estimate2 = static_cast<int>(round(mean + 0.5));
    int fuel1 = 0;
    int fuel2 = 0;
    for (auto&& x: crabs) {
        fuel1 += arithmeticSequenceSum(abs(x - estimate1));
        fuel2 += arithmeticSequenceSum(abs(x - estimate2));
    }
    std::cout << "Estimate1: " << estimate1 << " Fuel: " << fuel1 << std::endl;
    std::cout << "Estimate2: " << estimate2 << " Fuel: " << fuel2 << std::endl;
}

int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);

    std::vector<int> crabs;
    std::string num;
    while (std::getline(dataFile, num, ',')) {
        crabs.push_back(std::stoi(num));
    }
    dataFile.close();
    partOne(crabs);
    partTwo(crabs);
    partTwoBruteForce(crabs);
}