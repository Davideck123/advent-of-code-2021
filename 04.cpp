#include <iostream>
#include <array>
#include <vector>
#include <fstream>
#include <sstream>

using Board = std::array<std::array<std::pair<int, bool>, 5>, 5>;

class Bingo {
public:
    void add(std::istream& in);
    void remove(int boardIndex) { boards_.erase(boards_.begin() + boardIndex); }
    void update(int num);
    bool isEnd(int boardIndex) const;
    const std::vector<Board>& boards() const { return boards_; }
private:
    std::vector<Board> boards_;
};

void Bingo::add(std::istream& in) {
    Board b;
    int num;
    for (int i = 0; i < 25; ++i) {
        in >> num;
        b[i / 5][i % 5].first = num;
    }
    boards_.emplace_back(b);
}

void Bingo::update(int num) {
    for (int i = 0; i < boards_.size(); ++i) {
        for (int j = 0; j < 25; ++j) {
            if (!boards_[i][j / 5][j % 5].second && boards_[i][j / 5][j % 5].first == num) {
                boards_[i][j / 5][j % 5].second = true;
            }
        }
    }
}

bool Bingo::isEnd(int boardIndex) const {
    auto& board = boards_[boardIndex];

    for (int i = 0; i < 5; ++i) {
        //row check
        if (board[i][0].second && board[i][1].second &&
            board[i][2].second && board[i][3].second &&
            board[i][4].second)
            return true;
        //column check
        if (board[0][i].second && board[1][i].second &&
            board[2][i].second && board[3][i].second &&
            board[4][i].second)
            return true;
    }
    return false;
}

void partOne(Bingo& b, const std::string& line) {
    std::stringstream ss{ line };
    std::string num;
    bool done = false;
    while (!done)  {
        getline(ss, num, ',');
        b.update(std::stoi(num));

        for (int i = 0; i < b.boards().size(); ++i) {
            if (b.isEnd(i)) {
                int sum = 0;
                for (int j = 0; j < 25; ++j) {
                    if (!b.boards()[i][j / 5][j % 5].second)
                        sum += b.boards()[i][j / 5][j % 5].first;
                }
                std::cout << std::stoi(num) * sum << std::endl;
                done = true;
                break;
            }
        }
    }
}

void partTwo(Bingo& b, const std::string& line) {
    std::stringstream ss{ line };
    std::string num;
    bool done = false;
    while (!done)  {
        getline(ss, num, ',');
        b.update(std::stoi(num));

        std::vector<int> removeIndices;
        for (int i = 0; i < b.boards().size(); ++i) {
            if (b.isEnd(i) && b.boards().size() > 1) removeIndices.push_back(i);
            else if (b.isEnd(i)) {
                int sum = 0;
                for (int j = 0; j < 25; ++j) {
                    if (!b.boards()[i][j / 5][j % 5].second)
                        sum += b.boards()[i][j / 5][j % 5].first;
                }
                std::cout << std::stoi(num) * sum << std::endl;
                done = true;
                break;
            }
        }
        int reduce = 0;
        while (!removeIndices.empty()) {
            b.remove(removeIndices.back() - reduce++);
            removeIndices.pop_back();
        }
    }
}

int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);

    std::string line;
    dataFile >> line;

    Bingo b;
    while (dataFile.good()) {
        b.add(dataFile);

        dataFile.get();
        dataFile.get();
    }
    dataFile.close();

    partOne(b, line);
    partTwo(b, line);
}