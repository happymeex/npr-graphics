#ifndef UTIL
#define UTIL

#include <string>
#include <vector>

namespace util {
/**
 * Splits a given string into multiple substrings using a delimiter.
 *
 * @param s the string to be split
 * @param delim the delimiter character
 *
 * @return a vector of strings containing the substrings
 *
 * @throws None
 */
std::vector<std::string> split_str(const std::string &s, char delim);

} // namespace util

#endif