#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <set>

using BinarySet = std::multiset<std::string>;
using BitCounts = std::array<std::pair<int, int>, 12>;

class BinaryContainer {
public:
    void add(const std::string& s);
    void remove(const std::string& s);
    const BinarySet& nums() const { return nums_; }
    const BitCounts& zeroOneCounts() const { return zeroOneCounts_; }
private:
    BinarySet nums_;
    BitCounts zeroOneCounts_;
};

void BinaryContainer::add(const std::string& s) {
    nums_.emplace(s);

    for (int i = s.size() - 1; i >= 0; --i) {
        if (s[i] == '0') ++zeroOneCounts_[i].first;
        else ++zeroOneCounts_[i].second;
    }
}

void BinaryContainer::remove(const std::string& s) {
    nums_.erase(s);

    for (int i = s.size() - 1; i >= 0; --i) {
        if (s[i] == '0') --zeroOneCounts_[i].first;
        else --zeroOneCounts_[i].second;
    }
}

void partOne(const BinaryContainer& b) {
    std::string gamma, epsilon;
    for (auto&& x: b.zeroOneCounts()) {
        if (x.first > x.second) {
            gamma += '0';
            epsilon += '1';
        }
        else {
            gamma += '1';
            epsilon += '0';
        }
    }
    std::cout << std::stoi(gamma, nullptr, 2) *
        std::stoi(epsilon, nullptr, 2) << std::endl;
}

int countRating(BinaryContainer b, bool max) {
    for (int i = 0; i < b.zeroOneCounts().size(); ++i) {
        if (b.nums().size() == 1) break;
        auto nums = b.nums();
        auto counts = b.zeroOneCounts();
        char bit;
        if (counts[i].first > counts[i].second) bit = (max) ? '0' : '1';
        else bit = (max) ? '1' : '0';
        for (auto it = nums.begin(); it != nums.end(); ++it) {
            if (bit != (*it)[i]) b.remove(*it);
        }
    }
    return std::stoi(*b.nums().cbegin(), nullptr, 2);
}

void partTwo(const BinaryContainer& b) {
    int oxygenGenerator = countRating(b, true);
    int CO2Scrubber = countRating(b, false);
    std::cout << oxygenGenerator * CO2Scrubber << std::endl;
}

int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);

    BinaryContainer b;
    std::string line;
    while (dataFile >> line) b.add(line);
    dataFile.close();

    partOne(b);
    partTwo(b);
}