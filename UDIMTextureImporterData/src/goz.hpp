#pragma once

#include <string>
#include <vector>
#include "image.hpp"

using namespace Eigen;

class GoZ {
public:
    GoZ();
    ~GoZ();

    void read(std::string inputPath);
    void write(std::string outPath);
    void writeObj(std::string outPath, bool exportColor);
    void writePly(std::string outPath);
    void importVectorDisplacement(std::vector<std::string>& texture_paths);
    void importNormalDisplacement(std::vector<std::string>& texture_paths, double midValue);
    void importVertexColor(std::vector<std::string>& texture_paths, double gamma);

private:
    std::string name;
    std::vector<std::vector<float>> vertices;
    std::vector<std::vector<float>> vertexColor;
    std::vector<float> mask;
    std::vector<int> groups;
    std::vector<std::vector<int>> faces;
    std::vector<std::vector<std::pair<float, float>>> UVs;
    std::vector<Vector3f> normals;

    std::vector<Image> initTextures(std::vector<std::string>& texture_paths);
    void computeVertexNormals();
    void computeTangentBasis(const Vector3f& A,
        const Vector3f& B,
        const Vector3f& C,
        const Vector3f& H,
        const Vector3f& K,
        const Vector3f& L,
        Vector3f& T,
        Vector3f& U,
        Vector3f& N);
};
