#include <iostream>
#include <fstream>
#include <memory>
#include <stack>
#include <optional>
#include <cmath>
#include <vector>

class Node;
using NodePtr = std::unique_ptr<Node>;

class Node {
public:
    explicit Node(std::optional<int> value = std::nullopt,
                  NodePtr left = nullptr, NodePtr right = nullptr,
                  Node* parent = nullptr);

    void print() const;
    void reduce(bool verbose = false);
    int magnitude() const;
private:
    bool isReduced(int depth = 0) const;
    void explode(int depth = 0);
    bool split(bool used);
    static Node* getPredecessor(Node* n);
    static Node* getSuccessor(Node* n);

    std::optional<int> value_;
    NodePtr left_;
    NodePtr right_;
    Node* parent_;
};

Node::Node(std::optional<int> value, NodePtr left, NodePtr right, Node* parent) {
    value_ = value;
    left_ = std::move(left);
    right_ = std::move(right);
    parent_ = parent;
    if (left_ != nullptr) left_->parent_ = this;
    if (right_ != nullptr) right_->parent_ = this;
}

void Node::print() const {
    if (left_ != nullptr) {
        std::cout << '[';
        left_->print();
    }
    if (value_.has_value()) std::cout << value_.value();
    else std::cout << ',';
    if (right_ != nullptr) {
        right_->print();
        std::cout << ']';
    }
}

bool Node::isReduced(int depth) const {
    bool reduced = true;
    if (left_ != nullptr) reduced = left_->isReduced(depth + 1);

    if (!reduced || depth > 4 || (value_.has_value() && value_.value() >= 10)) return false;

    if (right_ != nullptr) reduced = right_->isReduced(depth + 1);
    return reduced;
}

Node* Node::getPredecessor(Node* n) {
    n = n->parent_;
    while (n->parent_ != nullptr) {
        if (n->parent_->right_.get() == n) {
            n = n->parent_->left_.get();
            while (n->right_ != nullptr) n = n->right_.get();
            return n;
        }
        n = n->parent_;
    }
    return nullptr;
}

Node* Node::getSuccessor(Node *n) {
    n = n->parent_;
    while (n->parent_ != nullptr) {
        if (n->parent_->left_.get() == n) {
            n = n->parent_->right_.get();
            while (n->left_ != nullptr) n = n->left_.get();
            return n;
        }
        n = n->parent_;
    }
    return nullptr;
}

void Node::explode(int depth) {
    if (depth == 4 && left_ != nullptr && right_ != nullptr) {
        int value = left_->value_.value();
        auto pred = getPredecessor(left_.get());
        left_ = nullptr;
        if (pred != nullptr) pred->value_.value() += value;

        value = right_->value_.value();
        auto succ = getSuccessor(right_.get());
        right_ = nullptr;
        if (succ != nullptr) succ->value_.value() += value;
        value_ = 0;
        return;
    }
    if (left_ != nullptr) left_->explode(depth + 1);
    if (right_ != nullptr) right_->explode(depth + 1);
}

void Node::reduce(bool verbose) {
    while (!isReduced()) {
        if (verbose) std::cout << "Reducing..." << std::endl;
        explode();
        if (verbose) {
            print();
            std::cout << std::endl;
        }
        split(false);
        if (verbose) {
            print();
            std::cout << std::endl;
        }
    }
}

bool Node::split(bool used) {
    if (left_ != nullptr && !used) used = left_->split(used);
    if (value_.has_value() && value_.value() >= 10) {
        left_ = std::make_unique<Node>(floor((double) value_.value() / 2), nullptr, nullptr, this);
        right_ = std::make_unique<Node>(ceil((double) value_.value() / 2), nullptr, nullptr, this);
        value_ = std::nullopt;
        return true;
    }
    if (right_ != nullptr && !used) used = right_->split(used);
    return used;
}

int Node::magnitude() const {
    int result;
    if (left_ != nullptr) result = 3 * left_->magnitude();
    if (value_.has_value()) return value_.value();
    if (right_ != nullptr) result += 2 * right_->magnitude();
    return result;
}

int charToInt(int c) {
    return c - '0';
}

NodePtr createNumber(const std::string& line) {
    std::stack<char> stack;
    std::stack<NodePtr> nodes;
    for (auto && c: line) {
        if (c == '[' || c == ',') stack.push(c);
        else if (std::isdigit(c)) nodes.push(std::make_unique<Node>(charToInt(c)));
        else if (c == ']') {
            NodePtr right = std::move(nodes.top());
            nodes.pop();

            NodePtr left = std::move(nodes.top());
            nodes.pop();

            nodes.push(std::make_unique<Node>(std::nullopt, std::move(left), std::move(right)));
            stack.pop();
        }
    }
    auto node = std::move(nodes.top());
    nodes.pop();
    return node;
}

void partOne(const std::vector<std::string>& lines) {
    auto num = createNumber(lines[0]);
    for (int i = 1; i < lines.size(); ++i) {
        auto nextNum = createNumber(lines[i]);
        num = std::make_unique<Node>(std::nullopt, std::move(num), std::move(nextNum));
        num->reduce();
    }
    num->print();
    std::cout << std::endl;
    std::cout << num->magnitude() << std::endl;
}

void partTwo(const std::vector<std::string>& lines) {
    int maxMagnitude = INT_MIN;
    for (int i = 0; i < lines.size(); ++i) {
        for (int j = 0; j < lines.size(); ++j) {
            if (j != i) {
                auto add = std::make_unique<Node>(std::nullopt, createNumber(lines[i]), createNumber(lines[j]));
                add->reduce();
                int magnitude = add->magnitude();
                if (magnitude > maxMagnitude) maxMagnitude = magnitude;
            }
        }
    }
    std::cout << maxMagnitude << std::endl;
}

int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(dataFile, line)) lines.push_back(line);
    dataFile.close();
    partOne(lines);
    partTwo(lines);
}