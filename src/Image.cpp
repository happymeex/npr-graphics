#include "Image.hpp"
#include <iostream>
#include <lodepng.h>

const glm::vec4 &Image::GetPixel(int x, int y) const
{
    if (x < width_ && y < height_)
    {
        return data_[y * width_ + x];
    }
    else
    {
        throw std::runtime_error(
            "Unable to get a pixel outside of image range.");
    }
}
void Image::SetPixel(int x, int y, glm::vec4 color)
{
    if (x < 0 || x >= width_ || y < 0 || y >= height_)
    {
        throw "Image SetPixel: index out of range";
    }
    color.r = std::min(1.0f, color.r);
    color.g = std::min(1.0f, color.g);
    color.b = std::min(1.0f, color.b);
    data_[y * width_ + x] = color;
}
void Image::SavePNG(const std::string &file_name) const
{
    std::cout << "Saving image to " << file_name << std::endl;
    std::vector<unsigned char> data(width_ * height_ * 4);
    for (int i = 0; i < width_ * height_; i++)
    {
        data[4 * i] = (unsigned int)(std::min(1.0f, data_[i].r) * 255.0);
        data[4 * i + 1] = (unsigned int)(std::min(1.0f, data_[i].g) * 255.0);
        data[4 * i + 2] = (unsigned int)(std::min(1.0f, data_[i].b) * 255.0);
        data[4 * i + 3] = (unsigned int)(data_[i].a * 255.0);
    }
    unsigned int error =
        lodepng::encode("out/" + file_name, data, width_, height_);
    if (error)
    {
        std::cout << "Error: " << lodepng_error_text(error) << std::endl;
    }
}

// Image processing functions

Image Image::ApplyLayer(
    const Image &image2,
    const std::function<glm::vec3(glm::vec3, glm::vec3)> &function) const
{
    if (image2.GetWidth() != width_ || image2.GetHeight() != height_)
    {
        throw std::invalid_argument("Multiplying differently sized images");
    }

    Image result{width_, height_};
    for (int x = 0; x < width_; ++x)
    {
        for (int y = 0; y < height_; ++y)
        {
            auto p1 = GetPixel(x, y);
            auto p2 = image2.GetPixel(x, y);
            auto blended = function(glm::vec3(p1), glm::vec3(p2));

            auto alpha1 = p1[3];
            auto alpha2 = p2[3];

            auto new_pixel = (1 - alpha2) * glm::vec3(p1) + alpha2 * blended;
            result.SetPixel(x, y, glm::vec4(new_pixel, alpha1));
        }
    }
    return result;
}

Image Image::ApplyFilter(const glm::mat3 &filter,
                         bool on_transparency) const
{

    Image result{width_, height_};

    float normalizer = 0.0f;

    for (int x = 0; x < 3; ++x)
    {
        for (int y = 0; y < 3; ++y)
        {
            normalizer += filter[x][y];
        }
    }

    if (normalizer == 0)
    {
        normalizer = 1.0f;
    }
    normalizer = 1.0f / normalizer;

    int upper_bound = static_cast<int>(width_) - 1;
    int lower_bound = static_cast<int>(height_) - 1;

    for (int x = 0; x < width_; ++x)
    {
        for (int y = 0; y < height_; ++y)
        {
            // Filter applied to transparency as well
            if (on_transparency)
            {
                glm::vec4 pixel{0, 0, 0, 0};
                for (int i = -1; i <= 1; ++i)
                {
                    for (int j = -1; j <= 1; ++j)
                    {
                        auto true_x = std::min(std::max(0, x + i), upper_bound);
                        auto true_y = std::min(std::max(0, y + j), lower_bound);
                        pixel += filter[i + 1][j + 1] *
                                 GetPixel(true_x, true_y);
                    }
                }
                result.SetPixel(x, y, normalizer * pixel);
            }
            else
            {
                glm::vec3 pixel{0, 0, 0};
                auto alpha = GetPixel(x, y)[3];
                for (int i = -1; i <= 1; ++i)
                {
                    for (int j = -1; j <= 1; ++j)
                    {
                        auto true_x = std::min(std::max(0, x + i), upper_bound);
                        auto true_y = std::min(std::max(0, y + j), lower_bound);
                        pixel += filter[i + 1][j + 1] *
                                 glm::vec3(GetPixel(true_x, true_y));
                    }
                }
                result.SetPixel(x, y, glm::vec4(normalizer * pixel, alpha));
            }
        }
    }
    return result;
}

Image Image::GetEdges() const
{
    const auto magnitude = [](glm::vec3 a, glm::vec3 b)
    {
        float val = std::sqrt(glm::dot(a, a) + glm::dot(b, b));
        return glm::vec3{val, val, val};
    };

    return ApplyFilter(GX).ApplyLayer(ApplyFilter(GY), magnitude);
}
