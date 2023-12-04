#include "util.hpp"
#include <sstream>

namespace util {
std::vector<std::string> split_str(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> result;
    while (std::getline(ss, item, delim)) {
        result.emplace_back(std::move(item));
    }
    return result;
}

}