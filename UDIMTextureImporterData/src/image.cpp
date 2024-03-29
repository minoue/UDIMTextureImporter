#define TINYEXR_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define LOG( message ) { Logger::write( message ); }

#include <cmath>
#include <filesystem>

#include "stb_image.h"
#include "image.hpp"
#include "util.hpp"
#include "logger.hpp"

#pragma warning(push, 0)
#include "tinyexr.h"
#pragma warning(pop)

Image::Image() {};

Image::Image(std::string path)
{
    read(path);
    this->isEmpty = false;
}

Image::~Image() {};

void Image::read(const std::string path)
{

    std::filesystem::path p = path;
    auto ext = p.extension();

    if (ext == ".exr") {
        loadExr(path);
    } else if (ext == ".tif" || ext == ".tiff") {
        loadTif(path);
    } else if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
        loadImg(path);
    } else {
        std::cout << "Not supported images" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Image::loadImg(const std::string& path)
{
    int& width = this->width;
    int& height = this->height;
    int& nchannels = this->nchannels;

    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nchannels, 3);

    LOG("Loading img : " + path);

    size_t img_size = static_cast<size_t>(width * height * nchannels);

    this->pixels.resize(img_size);

    if (data != NULL) {
        for (size_t i=0; i<img_size; i++) {
            int value = data[i];
            float float_value = float(value) / 255.0f;
            this->pixels[i] = float_value;
        }
    } else {
        stbi_image_free(data);
        std::cout << "err image loading img" << std::endl;
        exit(EXIT_FAILURE);
    }
    stbi_image_free(data);
}

void Image::loadExr(const std::string& path)
{
    int& width = this->width;
    int& height = this->height;
    int& nchannels = this->nchannels;
    nchannels = 4;

    LOG("Loading exr : " + path);
    float* out;
    const char* err = nullptr;

    int ret = LoadEXR(&out, &width, &height, path.c_str(), &err);
    if (ret != TINYEXR_SUCCESS) {
        if (err) {
            fprintf(stderr, "ERR : %s\n", err);
            FreeEXRErrorMessage(err); // release memory of error message.
            exit(EXIT_FAILURE);
        }
    } else {
        int size = width * height * nchannels;
        this->pixels.resize(static_cast<size_t>(size));
        for (int i = 0; i < size; i++) {
            float x = out[i];
            this->pixels[static_cast<size_t>(i)] = x;
        }

        free(out); // release memory of image data
    }
}

void Image::loadTif(const std::string& path)
{
    uint32_t width, height;
    uint16_t nc, bitDepth, row;

    LOG("Loading tif : " + path);

    TIFF* tif = TIFFOpen(path.c_str(), "r");

    if (tif) {
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &nc);
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitDepth);
        TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &row);

        this->nchannels = static_cast<int>(nc);
        this->width = static_cast<int>(width);
        this->height = static_cast<int>(height);

        uint32_t npixels = width * height;
        uint32_t img_size = npixels * nc;
        
        // Initialize iamge pixel array
        this->pixels.resize(img_size);
        std::fill(this->pixels.begin(), this->pixels.end(), 0.0f);

        if (row == 1 && bitDepth == 8) {
            // Use TIFFReadRGBAImage function instead of scanline
            // Tif images from certain softwares such as 3dcoat need to be processed
            // in this way. This is maybe because Rows/Strip is 1, but not sure.
            //
            uint32_t* raster = (uint32_t*)_TIFFmalloc(npixels * sizeof(uint32_t));
            if (raster == NULL) {
                std::cout << "err1" << std::endl;
            } else {
                if (TIFFReadRGBAImage(tif, width, height, raster, 0)) {
                    for (uint32_t y=0; y<width; y++) {
                        for (uint32_t x=0; x<height; x++) {

                            uint32_t elementNum = x + (width * y);

                            uint16_t R = (uint8_t)TIFFGetR(raster[elementNum]);
                            uint16_t G = (uint8_t)TIFFGetG(raster[elementNum]);
                            uint16_t B = (uint8_t)TIFFGetB(raster[elementNum]);
                            uint16_t A = (uint8_t)TIFFGetA(raster[elementNum]);

                            float r = float(R) / 255.0f;
                            float g = float(G) / 255.0f;
                            float b = float(B) / 255.0f;
                            float a = float(A) / 255.0f;

                            // flip pixels vertically
                            uint32_t targetNum = width * (height - y - 1) + x;

                            this->pixels[targetNum * nc + 0] = r;
                            this->pixels[targetNum * nc + 1] = g;
                            this->pixels[targetNum * nc + 2] = b;
                            this->pixels[targetNum * nc + 3] = a;
                        }
                    }
                } else {
                    std::cout << "err2" << std::endl;
                }
                _TIFFfree(raster);
            }
        } else {
            // For most standart tiff images
            //
            tdata_t buf = _TIFFmalloc(TIFFScanlineSize(tif));

            for (uint32_t row = 0; row < height; row++) {
                TIFFReadScanline(tif, buf, row);
                for (uint32_t col = 0; col < width; col++) {
                    float r, g, b;
                    if (bitDepth == 32) {
                        r = static_cast<float*>(buf)[col * nc + 0];
                        g = static_cast<float*>(buf)[col * nc + 1];
                        b = static_cast<float*>(buf)[col * nc + 2];
                    } else if (bitDepth == 16) {
                        uint16_t R = static_cast<uint16_t*>(buf)[col * nc + 0];
                        uint16_t G = static_cast<uint16_t*>(buf)[col * nc + 1];
                        uint16_t B = static_cast<uint16_t*>(buf)[col * nc + 2];
                        r = static_cast<float>(R) / static_cast<float>(65535.0);
                        g = static_cast<float>(G) / static_cast<float>(65535.0);
                        b = static_cast<float>(B) / static_cast<float>(65535.0);
                    } else {
                        uint16_t R = static_cast<uint8_t*>(buf)[col * nc + 0];
                        uint16_t G = static_cast<uint8_t*>(buf)[col * nc + 1];
                        uint16_t B = static_cast<uint8_t*>(buf)[col * nc + 2];
                        r = static_cast<float>(R) / static_cast<float>(255.0);
                        g = static_cast<float>(G) / static_cast<float>(255.0);
                        b = static_cast<float>(B) / static_cast<float>(255.0);
                    }
                    size_t targetNum = (col + (width * row)) * nc;
                    this->pixels[targetNum + 0] = r;
                    this->pixels[targetNum + 1] = g;
                    this->pixels[targetNum + 2] = b;
                }
            }
            _TIFFfree(buf);
        }
        TIFFClose(tif);
    } else {
        std::cout << "err" << std::endl;
        exit(EXIT_FAILURE);
    }
}

size_t ImageUtils::get_udim(const float u, const float v)
{
    size_t U = static_cast<size_t>(ceil(u));
    size_t V = static_cast<size_t>(floor(v)) * 10;
    return U + V;
}

Vector2f ImageUtils::localize_uv(const float& u, const float& v)
{
    float u_local = u - floor(u);
    float v_local = v - floor(v);
    Vector2f uv(u_local, v_local);
    return uv;
}

Vector3f ImageUtils::get_pixel_values(const float u, const float v, const std::vector<float>& texture, const int width, const int height, const int nchannel)
{
    // Get pixel values by bilinear filtering

    float float_width = static_cast<float>(width);
    float float_height = static_cast<float>(height);

    int x1 = static_cast<int>(std::round(float_width * u));
    int x2 = x1 + 1;
    int y1 = static_cast<int>(std::round(float_height * (1 - v)));
    int y2 = y1 + 1;

    size_t target_pixel1 = static_cast<size_t>(((width * (y1 - 1) + x1) - 1) * nchannel);
    size_t target_pixel2 = static_cast<size_t>(((width * (y1 - 1) + x2) - 1) * nchannel);
    size_t target_pixel3 = static_cast<size_t>(((width * (y2 - 1) + x1) - 1) * nchannel);
    size_t target_pixel4 = static_cast<size_t>(((width * (y2 - 1) + x2) - 1) * nchannel);

    Vector3f A;
    A << texture[target_pixel1], texture[target_pixel1 + 1], texture[target_pixel1 + 2];
    Vector3f B;
    B << texture[target_pixel2], texture[target_pixel2 + 1], texture[target_pixel2 + 2];
    Vector3f C;
    C << texture[target_pixel3], texture[target_pixel3 + 1], texture[target_pixel3 + 2];
    Vector3f D;
    D << texture[target_pixel4], texture[target_pixel4 + 1], texture[target_pixel4 + 2];

    float u1 = (static_cast<float>(x1) - 0.5f) / float_width;
    float u2 = (static_cast<float>(x2) - 0.5f) / float_width;
    float v1 = (static_cast<float>(y1) - 0.5f) / float_height;
    float v2 = (static_cast<float>(y2) - 0.5f) / float_height;

    Vector3f E = ((u2 - u) / (u2 - u1)) * A + ((u - u1) / (u2 - u1)) * B;
    Vector3f F = ((u2 - u) / (u2 - u1)) * C + ((u - u1) / (u2 - u1)) * D;
    Vector3f G = ((v2 - (1 - v)) / (v2 - v1)) * E + (((1 - v) - v1) / (v2 - v1)) * F;

    return G;
}
