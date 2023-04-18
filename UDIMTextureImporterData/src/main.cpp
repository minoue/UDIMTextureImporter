#include <iostream>
#include <string>
#include <vector>

#include "fromZ/readGoZFile.h"
#include "fromZ/writeGoZFile.h"

#include "goz.hpp"
#include "util.hpp"

int main()
{
    std::string inputPath("../test/milo.GoZ");
    std::string outputPath("../test/output.GoZ");

    // Load images
    //
    // Create texture path array
    std::vector<std::string> texture_paths;
    for (int i = 1; i < 8; i++) {
        std::string path = "/Volumes/CT1000MX/git/displacementImporter2/test/milouv_VD_100" + std::to_string(i) + ".tif";
        texture_paths.push_back(path);
    }

    // Find out the last number of the UDIM images
    int max_udim = 0;
    for (auto& path : texture_paths) {
        std::string texture_udim = Utils::pathGetUdim(path);
        int udim = std::stoi(texture_udim) - 1000;
        if (udim > max_udim) {
            max_udim = udim;
        }
    }
    std::cout << "Last udim: " << max_udim << std::endl;

    // Init texture data by the number of udim textures
    std::vector<Image> texture_data;
    texture_data.resize(static_cast<size_t>(max_udim));

    for (auto& path : texture_paths) {
        Image img(path);
        std::string texture_udim = Utils::pathGetUdim(path);
        int udim = stoi(texture_udim) - 1000;
        texture_data[static_cast<size_t>(udim - 1)] = img;
    }

    GoZ obj(inputPath);
    obj.displacement(texture_data);
    obj.write(outputPath);

    exit(EXIT_SUCCESS);
}
