#ifndef IMAGE
#define IMAGE

#include <glm/glm.hpp>
#include <string>
#include <vector>

const glm::mat3 GX(1, 2, 1, 0, 0, 0, -1, -2, -1);
const glm::mat3 GY(1, 0, -1, 2, 0, -2, 1, 0, -1);
const glm::mat3 GAUSS(1, 2, 1, 2, 4, 2, 1, 2, 1);

class Image
{
public:
  Image(int width, int height) : width_(width), height_(height)
  {
    data_ = std::vector<glm::vec4>(width * height);
  }

  int GetWidth() const { return width_; }

  int GetHeight() const { return height_; }

  const glm::vec4 &GetPixel(int x, int y) const;
  /**
   * Sets the color of a pixel at the specified coordinates in the image.
   *
   * @param x The x-coordinate of the pixel.
   * @param y The y-coordinate of the pixel.
   * @param color The color to set the pixel to.
   *
   * @throws const char* An exception is thrown if the coordinates are out of
   * range.
   */
  void SetPixel(int x, int y, glm::vec4 color);

  /**
   * Saves the image as a PNG file in the `out/` directory.
   * Prints an error message if the file cannot be saved.
   *
   * @param file_name The name of the file to save the image to.
   */
  void SavePNG(const std::string &file_name) const;

  Image ApplyLayer(const Image &image2,
                   const std::function<glm::vec3(glm::vec3, glm::vec3)> &function) const;

  Image ApplyFilter(const glm::mat3 &filter,
                    bool on_transparency = false) const;

  Image GetEdges() const;

private:
  int width_;
  int height_;
  /**
   * Row-major order pixel RGBA data for the image.
   * All values are in the range [0, 1].
   */
  std::vector<glm::vec4> data_;
};

#endif