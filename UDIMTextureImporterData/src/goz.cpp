#include "FromZ/src/readGoZFile.h"
#include "FromZ/src/writeGoZFile.h"

#include "goz.hpp"
#include "util.hpp"

GoZ::GoZ() {};

GoZ::~GoZ() {};

void GoZ::read(std::string inputPath)
{
    std::cout << "Reading GoZ File..." << std::endl;
    FromZ::readGoZFile(inputPath, name, vertices, faces, UVs, vertexColor, mask, groups);

    std::cout << "Initializing Vertex Normals..." << std::endl;
    computeVertexNormals();
}

void GoZ::write(std::string outPath)
{
    std::cout << "Writing GoZ File..." << std::endl;
    FromZ::writeGoZFile(outPath, name, vertices, faces, UVs, vertexColor, mask, groups);
}

void GoZ::computeVertexNormals()
{

    this->normals.resize(this->vertices.size());
    Vector3f zeroVec(0, 0, 0);
    std::fill(normals.begin(), normals.end(), zeroVec);

    // Set vertex normals
    size_t numFaces = this->faces.size();
    for (size_t i = 0; i < numFaces; i++) {
        std::vector<int>& faceVertices = this->faces[i];

        size_t numFaceVertices = faceVertices.size();

        for (size_t j = 0; j < numFaceVertices; j++) {

            int currentVertexID, nextVertexID, nextNextVertexID;

            if (numFaceVertices - j == 2) {
                // One before the last
                currentVertexID = faceVertices[j];
                nextVertexID = faceVertices[j + 1];
                nextNextVertexID = faceVertices[0];
            } else if (numFaceVertices - j == 1) {
                // last
                currentVertexID = faceVertices[j];
                nextVertexID = faceVertices[0];
                nextNextVertexID = faceVertices[1];
            } else {
                currentVertexID = faceVertices[j];
                nextVertexID = faceVertices[j + 1];
                nextNextVertexID = faceVertices[j + 2];
            }

            std::vector<float>& p0 = this->vertices[static_cast<size_t>(currentVertexID)];
            std::vector<float>& p1 = this->vertices[static_cast<size_t>(nextVertexID)];
            std::vector<float>& p2 = this->vertices[static_cast<size_t>(nextNextVertexID)];

            Vector3f P0(p0.data());
            Vector3f P1(p1.data());
            Vector3f P2(p2.data());

            // Re-calculate Normals
            Vector3f E1 = P1 - P0;
            Vector3f E2 = P2 - P0;
            Vector3f faceNormal = E1.cross(E2);

            this->normals[static_cast<size_t>(currentVertexID)] += faceNormal;
        }
    }
    for (auto& n : this->normals) {
        n.normalize();
    }
}

// https://stackoverflow.com/questions/5255806/how-to-calculate-tangent-and-binorma
void GoZ::computeTangentBasis(const Vector3f& A,
    const Vector3f& B,
    const Vector3f& C,
    const Vector3f& H,
    const Vector3f& K,
    const Vector3f& L,
    Vector3f& T,
    Vector3f& U,
    Vector3f& N)
{

    Vector3f D = B - A;
    Vector3f E = C - A;
    Vector3f F = K - H;
    Vector3f G = L - H;

    MatrixXf DE(2, 3);
    DE << D.x(), D.y(), D.z(),
        E.x(), E.y(), E.z();

    Matrix2f FG;
    FG << F.x(), F.y(),
        G.x(), G.y();

    MatrixXf result(2, 3);
    result = FG.inverse() * DE;

    Vector3f new_T = result.row(0);
    new_T -= N * new_T.dot(N);
    new_T.normalize();
    Vector3f bitangent = N.cross(new_T);

    T = new_T;
    U = bitangent.normalized();
}

std::vector<Image> GoZ::initTextures(std::vector<std::string>& texture_paths)
{
    // Find out the last number of the UDIM images
    int max_udim = 0;
    for (auto& path : texture_paths) {
        std::string texture_udim = Utils::pathGetUdim(path);
        int udim = std::stoi(texture_udim) - 1000;
        if (udim > max_udim) {
            max_udim = udim;
        }
    }

    // Init texture data by the number of udim textures
    std::vector<Image> textures;
    textures.resize(static_cast<size_t>(max_udim));
    for (auto& path : texture_paths) {
        Image img(path);
        std::string texture_udim = Utils::pathGetUdim(path);
        int udim = stoi(texture_udim) - 1000;
        textures[static_cast<size_t>(udim - 1)] = img;
    }
    return textures;
}

void GoZ::importVectorDisplacement(std::vector<std::string>& texture_paths)
{

    std::vector<Image> textures = initTextures(texture_paths);

    // Vector Displacement
    std::vector<std::vector<float>> outVertices;
    outVertices.resize(this->vertices.size());

    size_t numFaces = this->faces.size();
    for (size_t i = 0; i < numFaces; i++) {
        std::vector<int>& faceVertices = this->faces[i];
        std::vector<std::pair<float, float>>& faceUVs = this->UVs[i];

        size_t numFaceVertices = faceVertices.size();

        for (size_t j = 0; j < numFaceVertices; j++) {

            int currentVertexID, nextVertexID, nextNextVertexID;
            Vector3f uv0;
            Vector3f uv1;
            Vector3f uv2;

            if (numFaceVertices - j == 2) {
                // One before the last
                currentVertexID = faceVertices[j];
                nextVertexID = faceVertices[j + 1];
                nextNextVertexID = faceVertices[0];
                uv0 << faceUVs[j].first, faceUVs[j].second, 0.0;
                uv1 << faceUVs[j + 1].first, faceUVs[j + 1].second, 0.0;
                uv2 << faceUVs[0].first, faceUVs[0].second, 0.0;
            } else if (numFaceVertices - j == 1) {
                // last
                currentVertexID = faceVertices[j];
                nextVertexID = faceVertices[0];
                nextNextVertexID = faceVertices[1];
                uv0 << faceUVs[j].first, faceUVs[j].second, 0.0;
                uv1 << faceUVs[0].first, faceUVs[0].second, 0.0;
                uv2 << faceUVs[1].first, faceUVs[1].second, 0.0;
            } else {
                currentVertexID = faceVertices[j];
                nextVertexID = faceVertices[j + 1];
                nextNextVertexID = faceVertices[j + 2];
                uv0 << faceUVs[j].first, faceUVs[j].second, 0.0;
                uv1 << faceUVs[j + 1].first, faceUVs[j + 1].second, 0.0;
                uv2 << faceUVs[j + 2].first, faceUVs[j + 2].second, 0.0;
            }

            std::vector<float>& p0 = this->vertices[static_cast<size_t>(currentVertexID)];
            std::vector<float>& p1 = this->vertices[static_cast<size_t>(nextVertexID)];
            std::vector<float>& p2 = this->vertices[static_cast<size_t>(nextNextVertexID)];

            Vector3f P0(p0.data());
            Vector3f P1(p1.data());
            Vector3f P2(p2.data());

            Vector3f T, B, N;
            N = this->normals[static_cast<size_t>(currentVertexID)];
            N.normalize();

            computeTangentBasis(P0, P1, P2, uv0, uv1, uv2, T, B, N);

            Matrix3f mat;
            mat << T.x(), T.y(), T.z(),
                N.x(), N.y(), N.z(),
                B.x(), B.y(), B.z();

            float u = uv0.x();
            float v = uv0.y();

            size_t udim = ImageUtils::get_udim(u, v);

            Vector3f displace;

            if (udim > textures.size()) {
                // If UVs are outside of the given UDIM range, use same point
                displace << 0, 0, 0;
            } else {
                Image& img = textures[udim - 1];
                if (img.isEmpty) {
                    // If UVs are within the given UDIM range but has no textures, use same point
                    displace << 0, 0, 0;
                } else {
                    int width = img.width;
                    int height = img.height;
                    int channels = img.nchannels;

                    Vector2f local_uv = ImageUtils::localize_uv(u, v);
                    Vector3f rgb;
                    rgb = ImageUtils::get_pixel_values(local_uv.x(), local_uv.y(), img.pixels, width, height, channels);
                    displace = rgb.transpose() * mat;
                }
            }
            Vector3f new_pp = P0 + displace;
            std::vector<float> xyz = { new_pp.x(), new_pp.y(), new_pp.z() };
            outVertices[static_cast<size_t>(currentVertexID)] = xyz;
        }
    }
    this->vertices = outVertices;
}

void GoZ::importNormalDisplacement(std::vector<std::string>& texture_paths)
{

    std::vector<Image> textures = initTextures(texture_paths);

    // Vector Displacement
    std::vector<std::vector<float>> outVertices;
    outVertices.resize(this->vertices.size());

    size_t numFaces = this->faces.size();
    for (size_t i = 0; i < numFaces; i++) {
        std::vector<int>& faceVertices = this->faces[i];
        std::vector<std::pair<float, float>>& faceUVs = this->UVs[i];

        size_t numFaceVertices = faceVertices.size();

        for (size_t j = 0; j < numFaceVertices; j++) {

            int vertexID = faceVertices[j];
            std::vector<float>& P = this->vertices[static_cast<size_t>(vertexID)];

            Vector3f uv;
            uv << faceUVs[j].first, faceUVs[j].second, 0.0;

            Vector3f P0(P.data());

            Vector3f N;
            N = this->normals[static_cast<size_t>(vertexID)];
            N.normalize();

            float u = uv.x();
            float v = uv.y();

            size_t udim = ImageUtils::get_udim(u, v);

            Vector3f rgb;

            if (udim > textures.size()) {
                // If UVs are outside of the given UDIM range, use same point
                rgb << 0, 0, 0;
            } else {
                Image& img = textures[udim - 1];
                if (img.isEmpty) {
                    // If UVs are within the given UDIM range but has no textures, use same point
                    rgb << 0, 0, 0;
                } else {
                    int width = img.width;
                    int height = img.height;
                    int channels = img.nchannels;

                    Vector2f local_uv = ImageUtils::localize_uv(u, v);
                    Vector3f rgb;
                    rgb = ImageUtils::get_pixel_values(local_uv.x(), local_uv.y(), img.pixels, width, height, channels);
                }
            }
            Vector3f new_pp = P0 + (N * rgb.x());
            std::vector<float> xyz = { new_pp.x(), new_pp.y(), new_pp.z() };
            outVertices[static_cast<size_t>(vertexID)] = xyz;
        }
    }
    this->vertices = outVertices;
}

void GoZ::importVertexColor(std::vector<std::string>& texture_paths, double gamma)
{

    std::vector<Image> textures = initTextures(texture_paths);

    // Vertex Color
    std::vector<std::vector<float>> outColor;
    outColor.resize(this->vertices.size());

    size_t numFaces = this->faces.size();
    for (size_t i = 0; i < numFaces; i++) {
        std::vector<int>& faceVertices = this->faces[i];
        std::vector<std::pair<float, float>>& faceUVs = this->UVs[i];

        size_t numFaceVertices = faceVertices.size();

        for (size_t j = 0; j < numFaceVertices; j++) {

            int vertexID = faceVertices[j];
            std::vector<float>& P = this->vertices[static_cast<size_t>(vertexID)];

            Vector3f uv;
            uv << faceUVs[j].first, faceUVs[j].second, 0.0;

            Vector3f P0(P.data());

            Vector3f N;
            N = this->normals[static_cast<size_t>(vertexID)];
            N.normalize();

            float u = uv.x();
            float v = uv.y();

            size_t udim = ImageUtils::get_udim(u, v);

            Vector3f rgb;
            float alpha = 1.0;

            if (udim > textures.size()) {
                // If UVs are outside of the given UDIM range, no color
                rgb << 0, 0, 0;
                alpha = 0.0;
            } else {
                Image& img = textures[udim - 1];
                if (img.isEmpty) {
                    // If UVs are within the given UDIM range but has no textures, no color
                    rgb << 0, 0, 0;
                    alpha = 0.0;
                } else {
                    int width = img.width;
                    int height = img.height;
                    int channels = img.nchannels;

                    Vector2f local_uv = ImageUtils::localize_uv(u, v);
                    rgb = ImageUtils::get_pixel_values(local_uv.x(), local_uv.y(), img.pixels, width, height, channels);
                }
            }
            float gammaCorrection = static_cast<float>(1.0 / gamma);
            std::vector<float> col = {pow(rgb.x(), gammaCorrection),
                                      pow(rgb.y(), gammaCorrection),
                                      pow(rgb.z(), gammaCorrection),
                                      alpha };

            outColor[static_cast<size_t>(vertexID)] = col;
        }
    }
    this->vertexColor = outColor;
}

void GoZ::importMask(std::vector<std::string>& texture_paths)
{

    std::vector<Image> textures = initTextures(texture_paths);

    // Vertex Color
    std::vector<float> outMask;
    outMask.resize(this->mask.size());

    size_t numFaces = this->faces.size();
    for (size_t i = 0; i < numFaces; i++) {
        std::vector<int>& faceVertices = this->faces[i];
        std::vector<std::pair<float, float>>& faceUVs = this->UVs[i];

        size_t numFaceVertices = faceVertices.size();

        for (size_t j = 0; j < numFaceVertices; j++) {

            int vertexID = faceVertices[j];
            std::vector<float>& P = this->vertices[static_cast<size_t>(vertexID)];

            Vector3f uv;
            uv << faceUVs[j].first, faceUVs[j].second, 0.0;

            Vector3f P0(P.data());

            Vector3f N;
            N = this->normals[static_cast<size_t>(vertexID)];
            N.normalize();

            float u = uv.x();
            float v = uv.y();

            size_t udim = ImageUtils::get_udim(u, v);

            Vector3f rgb;

            if (udim > textures.size()) {
                // If UVs are outside of the given UDIM range, use same point
                rgb << 0, 0, 0;
            } else {
                Image& img = textures[udim - 1];
                if (img.isEmpty) {
                    // If UVs are within the given UDIM range but has no textures, use same point
                    rgb << 0, 0, 0;
                } else {
                    int width = img.width;
                    int height = img.height;
                    int channels = img.nchannels;

                    Vector2f local_uv = ImageUtils::localize_uv(u, v);
                    rgb = ImageUtils::get_pixel_values(local_uv.x(), local_uv.y(), img.pixels, width, height, channels);
                }
            }
            float maskValue = rgb.x();
            outMask[static_cast<size_t>(vertexID)] = maskValue;
        }
    }
    this->mask = outMask;
}
