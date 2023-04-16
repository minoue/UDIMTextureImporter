#pragma once

#include "eigen/Eigen/Core"
#include "eigen/Eigen/Dense"
#include "eigen/Eigen/LU"
#include "tiffio.hxx"
#include <vector>

using namespace Eigen;

class Image {
public:
    Image();
    Image(std::string path);
    ~Image();
    int width;
    int height;
    int nchannels;
    std::vector<float> pixels;
    void read(const std::string path);
    bool isEmpty = true;

private:
    void loadExr(const std::string& path);
    void loadTif(const std::string& path);
};

namespace ImageUtils {
size_t get_udim(const float u, const float v);
Vector2f localize_uv(const float& u, const float& v);
Vector3f get_pixel_values(const float u, const float v, const std::vector<float>& texture, const int width, const int height, const int nchannel);
}
