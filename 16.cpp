#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <numeric>
#include <algorithm>

class PacketDecoder {
public:
    explicit PacketDecoder(const std::string& packet) : packet_(packet) {}
    long long decode();
    int getSum() const { return versionsSum; }
private:
    long long getValue(int substrLen);

    const std::string& packet_;
    int index{ 0 };
    int versionsSum{ 0 };
};

long long PacketDecoder::getValue(int substrLen) {
    long long value = std::stoll(packet_.substr(index, substrLen), nullptr, 2);
    index += substrLen;
    return value;
}

long long PacketDecoder::decode() {
    int version = (int) getValue(3);
    versionsSum += version;
    int id = (int) getValue(3);
    if (id == 4) {
        long long value = 0;
        while (packet_[index++] == '1') {
            value += getValue(4);
            value <<= 4;
        }
        return value + getValue(4);
    }
    std::vector<long long> results;
    if (packet_[index++] == '1') {
        int subPackets = (int) getValue(11);
        for (int i = 0; i < subPackets; ++i) results.push_back(decode());
    }
    else {
        int subPacketsLength = (int) getValue(15);
        int maxIndex = index + subPacketsLength;
        while (index < maxIndex) results.push_back(decode());
    }
    if (id == 5) return (results[0] > results[1]) ? 1 : 0;
    if (id == 6) return (results[0] < results[1]) ? 1 : 0;
    if (id == 7) return (results[0] == results[1]) ? 1 : 0;
    if (id == 0) return std::accumulate(results.cbegin(), results.cend(), 0LL);
    if (id == 1) return std::accumulate(results.cbegin(), results.cend(), 1LL, std::multiplies<>());
    if (id == 2) return *std::min_element(results.cbegin(), results.cend());
    return *std::max_element(results.cbegin(), results.cend());
}

std::string convertPacket(const std::string& packet) {
    const std::map<char, std::string> HEX_TO_BIN{ {'0', "0000"},
                                                  {'1', "0001"},
                                                  {'2', "0010"},
                                                  {'3', "0011"},
                                                  {'4', "0100"},
                                                  {'5', "0101"},
                                                  {'6', "0110"},
                                                  {'7', "0111"},
                                                  {'8', "1000"},
                                                  {'9', "1001"},
                                                  {'A', "1010"},
                                                  {'B', "1011"},
                                                  {'C', "1100"},
                                                  {'D', "1101"},
                                                  {'E', "1110"},
                                                  {'F', "1111"} };
    std::string converted;
    for (auto&& c: packet) converted += HEX_TO_BIN.at(c);
    return converted;
}

int main(int argc, char* argv[]) {
    std::ifstream dataFile(argv[1]);
    std::string packet;
    std::getline(dataFile, packet);
    dataFile.close();

    std::string converted = convertPacket(packet);

    PacketDecoder p(converted);
    std::cout << "Result: " << p.decode() << std::endl;
    std::cout << "Version sum: " << p.getSum() << std::endl;
}