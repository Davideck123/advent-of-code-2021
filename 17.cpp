#include <iostream>
#include <fstream>
#include <set>
#include <optional>

using Point = std::pair<int, int>;

class Launcher {
public:
    explicit Launcher(std::ifstream& in);
    int xLow() const { return xLow_; }
    int xHigh() const { return xHigh_; }
    int yLow() const { return yLow_; }
    int yHigh() const { return yHigh_; }
    int highestY() const { return highestY_; }
    int velocitiesCount() const { return (int) initVelocities_.size(); }
    void computeVelocities();
private:
    std::optional<int> simulate(int velocityX, int velocityY);

    int xLow_;
    int xHigh_;
    int yLow_;
    int yHigh_;
    int highestY_{ INT_MIN };
    std::set<Point> initVelocities_;
};

Launcher::Launcher(std::ifstream& in) {
    for (int i = 0; i < 4; ++i) {
        std::string s;
        char c = (char) in.get();
        while (!std::isdigit(c) && c != '-') c = (char) in.get();
        while (std::isdigit(c) || c == '-') {
            s += c;
            c = (char) in.get();
        }
        switch (i) {
            case 0:
                xLow_ = std::stoi(s);
                break;
            case 1:
                xHigh_ = std::stoi(s);
                break;
            case 2:
                yLow_ = std::stoi(s);
                break;
            default:
                yHigh_ = std::stoi(s);
                break;
        }
    }
}

std::optional<int> Launcher::simulate(int velocityX, int velocityY) {
    int x = 0, y = 0;
    bool inside = false;
    int highestY = INT_MIN;
    while (y >= yLow_) {
        x += velocityX;
        y += velocityY;
        if (highestY < y) highestY = y;
        if (!inside && xLow_ <= x && x <= xHigh_ && yLow_ <= y && y <= yHigh_) inside = true;
        if (velocityX > 0) --velocityX;
        else if (velocityX < 0) ++velocityX;
        --velocityY;
    }
    if (inside) {
        if (highestY_ < highestY) highestY_ = highestY;
        return highestY_;
    }
    else return std::nullopt;
}

void Launcher::computeVelocities() {
    for (int x = 0; x <= xHigh_; ++x) {
        for (int y = yLow_; y <= abs(yLow_); ++y) {
            auto result = simulate(x, y);
            if (result.has_value()) initVelocities_.emplace(x, y);
        }
    }
}

int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);
    Launcher launcher(dataFile);
    dataFile.close();

    launcher.computeVelocities();
    std::cout << "Highest Y: " << launcher.highestY() << std::endl;
    std::cout << "Velocities Count: " << launcher.velocitiesCount() << std::endl;
}