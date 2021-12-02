#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <deque>

class Increase {
public:
    bool operator()(int x);
    int count() const { return count_; }
private:
    int prev_{ -1 };
    int count_{ 0 };
    bool first_{ true };
};

bool Increase::operator()(int x) {
    if (!first_) {
        if (x > prev_) {
            prev_ = x;
            ++count_;
            return true;
        }
    }
    else first_ = false;
    prev_ = x;
    return false;
}

class SumIncrease {
public:
    SumIncrease(int elemCount) : elemCount_(elemCount) {}
    bool operator()(int x);
    int count() const { return count_; }
private:
    int elemCount_;
    int prev_{ -1 };
    int count_{ 0 };
    bool first_{ true };
    std::deque<int> values_;
};

bool SumIncrease::operator()(int x) {
    values_.push_back(x);
    if (values_.size() < elemCount_) return false;
    if (!first_) {
        values_.pop_front();
        int sum = std::accumulate(values_.cbegin(), values_.cend(), 0);
        if (sum > prev_) {
            prev_ = sum;
            ++count_;
            return true;
        }
        prev_ = sum;
    }
    else {
        prev_ = std::accumulate(values_.cbegin(), values_.cend(), 0);
        first_ = false;
    }
    return false;
}

void partOne(const std::vector<int>& depths) {
    auto functor = std::for_each(depths.cbegin(), depths.cend(), Increase());
    std::cout << "Part One: " << functor.count() << std::endl;
}

void partTwo(const std::vector<int>& depths) {
    auto functor = std::for_each(depths.cbegin(), depths.cend(), SumIncrease(3));
    std::cout << "Part Two: " << functor.count() << std::endl;
}

int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);
    std::string line;
    std::vector<int> depths;

    while (std::getline(dataFile, line)) {
        depths.push_back(std::stoi(line));
    }
    dataFile.close();

    partOne(depths);
    partTwo(depths);
}