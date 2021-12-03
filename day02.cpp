#include <fstream>
#include <string>
#include <iostream>

class Submarine {
public:
    void move(std::istream& input, bool aim = false);
    int horizontal() const { return horizontal_; }
    int depth() const { return depth_; }
private:
    int horizontal_{ 0 };
    int depth_{ 0 };
    int aim_{ 0 };
};

void Submarine::move(std::istream& input, bool aim) {
    std::string keyword, value;
    while (input >> keyword >> value) {
        if (keyword == "forward") {
            if (aim) depth_ += aim_ * std::stoi(value);
            horizontal_ += std::stoi(value);
        }
        else if (keyword == "down") {
            if (aim) aim_ += std::stoi(value);
            else depth_ += std::stoi(value);
        }
        else if (keyword == "up") {
            if (aim) aim_ -= std::stoi(value);
            else depth_ -= std::stoi(value);
        }
    }
}

void taskOne(std::istream& input) {
    Submarine s;
    s.move(input);
    std::cout << "Part One: " << s.horizontal() * s.depth() << std::endl;
}

void taskTwo(std::istream& input) {
    Submarine s;
    s.move(input, true);
    std::cout << "Part Two: " << s.horizontal() * s.depth() << std::endl;
}

int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);
    taskOne(dataFile);

    // return to the begging of the file
    dataFile.clear();
    dataFile.seekg(0);

    taskTwo(dataFile);
    dataFile.close();
}