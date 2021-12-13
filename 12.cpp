#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

using CaveMap = std::map<std::string, std::set<std::string>>;

class PathSearcher {
public:
    bool isSmall(const std::string& cave) const;
    CaveMap& getCaveMap() { return caveMap_; }
    int getPathCount() const { return pathCount_; }
    void search();
    bool containsCave(const std::string& cave) const;
    void switchTask() { partTwo_ = !partTwo_; pathCount_ = 0; }
private:
    CaveMap caveMap_;
    int pathCount_{ 0 };
    std::vector<std::string> cavesUsed_{ "start" };
    bool smallTwiceUsed_{ false };
    bool partTwo_{ false };
};

bool PathSearcher::isSmall(const std::string& cave) const {
    for (auto&& x: cave) {
        if (std::isupper(x)) return false;
    }
    return true;
}

bool PathSearcher::containsCave(const std::string& cave) const {
    return std::find(cavesUsed_.cbegin(), cavesUsed_.cend(), cave) != cavesUsed_.cend();
}

void PathSearcher::search() {
    if (cavesUsed_.back() == "end") {
        //for (auto&& x: cavesUsed_) std::cout << x << " ";
        //std::cout << std::endl;
        ++pathCount_;
        return;
    }
    for (auto&& x: caveMap_[cavesUsed_.back()]) {
        if (x != "start") {
            if (!isSmall(x) || !containsCave(x)) {
                cavesUsed_.emplace_back(x);
                search();
                cavesUsed_.pop_back();
            }
            else if (partTwo_ && !smallTwiceUsed_) {
                smallTwiceUsed_ = true;
                cavesUsed_.emplace_back(x);
                search();
                cavesUsed_.pop_back();
                smallTwiceUsed_ = false;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);
    PathSearcher p;
    while (dataFile.good()) {
        std::string s1, s2;
        std::getline(dataFile, s1, '-');
        std::getline(dataFile, s2);
        p.getCaveMap()[s1].emplace(s2);
        p.getCaveMap()[s2].emplace(s1);
    }
    dataFile.close();

    //part one
    p.search();
    std::cout << p.getPathCount() << std::endl;
    //part two
    p.switchTask();
    p.search();
    std::cout << p.getPathCount() << std::endl;
}