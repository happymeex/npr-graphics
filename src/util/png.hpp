#ifndef UTIL_PNG
#define UTIL_PNG

#include <lodepng.h>
#include <string>
#include <vector>

void save_png(const std::string &path, const std::vector<unsigned char> &image,
              int width, int height);

#endif