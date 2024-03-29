#define _CRT_SECURE_NO_WARNINGS
#define LOG( message ) { Logger::write( message ); }

#include <stdio.h>

#pragma warning(push, 0)
#include "FromZ/src/readGoZFile.h"
#include "FromZ/src/writeGoZFile.h"
#pragma warning(pop)

#include "goz.hpp"
#include "timer.hpp"
#include "util.hpp"
#include "logger.hpp"

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
    FromZ::writeGoZFile(outPath, name, vertices, faces, UVs, vertexColor, mask, groups);
}

void GoZ::computeVertexNormals()
{
    LOG("calculating vertex normal.")

    Timer timer;
    timer.start();

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

    LOG("calculated vertex normal sucessfully.")
    timer.showDuration("Vertex normal calculated in ");
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

    Timer timer;
    timer.start();

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

    timer.showDuration("Finished loading textures in ");

    return textures;
}

void GoZ::importVectorDisplacement(std::vector<std::string>& texture_paths)
{
    LOG("Applying Vector Displacement.")

    Timer timer;
    timer.start();

    std::vector<Image> textures = initTextures(texture_paths);

    // Vector Displacement
    std::vector<std::vector<float>> outVertices = this->vertices;

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

            if (udim > textures.size()) {
                // If UVs are outside of the given UDIM range, use same point
                continue;
            }

            Image& img = textures[udim - 1];

            if (!img.isEmpty) {
                Vector2f local_uv = ImageUtils::localize_uv(u, v);
                Vector3f rgb = ImageUtils::get_pixel_values(
                    local_uv.x(),
                    local_uv.y(),
                    img.pixels,
                    img.width,
                    img.height,
                    img.nchannels);
                Vector3f displace = rgb.transpose() * mat;
                Vector3f new_pp = P0 + displace;
                std::vector<float> xyz = { new_pp.x(), new_pp.y(), new_pp.z() };
                outVertices[static_cast<size_t>(currentVertexID)] = xyz;
            }
        }
    }
    this->vertices = outVertices;

    timer.showDuration("Finished Vector Displacement in ");
    LOG("Finished Apllying Vector Displacement.")
}

void GoZ::importNormalDisplacement(std::vector<std::string>& texture_paths, double midValue)
{
    LOG("Applying Normal Displacement.")

    Timer timer;
    timer.start();

    std::cout << "Mid value : " << midValue << std::endl;

    std::vector<Image> textures = initTextures(texture_paths);

    std::vector<std::vector<float>> outVertices = this->vertices;

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

            if (udim > textures.size()) {
                // If UVs are outside of the given UDIM range, use same point
                continue;
            }

            Image& img = textures[udim - 1];

            if (!img.isEmpty) {
                Vector2f local_uv = ImageUtils::localize_uv(u, v);
                Vector3f rgb = ImageUtils::get_pixel_values(
                    local_uv.x(),
                    local_uv.y(),
                    img.pixels,
                    img.width,
                    img.height,
                    img.nchannels);
                Vector3f new_pp = P0 + (N * (rgb.x() - midValue));
                std::vector<float> xyz = { new_pp.x(), new_pp.y(), new_pp.z() };
                outVertices[static_cast<size_t>(vertexID)] = xyz;
            }
        }
    }
    this->vertices = outVertices;

    timer.showDuration("Finished Normal Displacement in ");
    LOG("Finished Apllying Normal Displacement.")
}

void GoZ::importVertexColor(std::vector<std::string>& texture_paths, double gamma)
{

    Timer timer;
    timer.start();

    std::vector<Image> textures = initTextures(texture_paths);

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

            float u = uv.x();
            float v = uv.y();

            size_t udim = ImageUtils::get_udim(u, v);

            if (udim > textures.size()) {
                // If UVs are outside of the given UDIM range, no color changes
                continue;
            }

            Image& img = textures[udim - 1];

            if (!img.isEmpty) {
                Vector3f rgb;
                Vector2f local_uv = ImageUtils::localize_uv(u, v);
                rgb = ImageUtils::get_pixel_values(
                    local_uv.x(),
                    local_uv.y(),
                    img.pixels,
                    img.width,
                    img.height,
                    img.nchannels);
                float gammaCorrection = static_cast<float>(1.0 / gamma);

                std::vector<float> col = { pow(rgb.x(), gammaCorrection),
                    pow(rgb.y(), gammaCorrection),
                    pow(rgb.z(), gammaCorrection),
                    1.0 };

                this->vertexColor[static_cast<size_t>(vertexID)] = col;
            }
        }
    }

    timer.showDuration("Finished color to polypaint in ");
}

void GoZ::writeObj(std::string out_path, bool exportColor) {

    LOG("Exporting obj file for ZBruush...");

    FILE* fp;
    fp = fopen(out_path.c_str(), "w");

    if (fp == NULL) {
        printf("%s file cannot be opened\n", out_path.c_str());
        exit(EXIT_FAILURE);
    }

    char line[128];
  
    // Export Vertex positions
    size_t numVerts = this->vertices.size();
    
    for (size_t i=0; i<numVerts; i++) {
        std::vector<float>& v = this->vertices[i];
        std::vector<float>& Cd = this->vertexColor[i];
        line[0] = '\0'; // clear
                        //
        if (exportColor) {
            snprintf(
                line,
                sizeof(line),
                "v %f %f %f %f %f %f\n", v[0], -v[1], -v[2], Cd[0], Cd[1], Cd[2]); // ZBrush is flipped in y and z by default so negate them
        } else {
            snprintf(line, sizeof(line), "v %f %f %f\n", v[0], -v[1], -v[2]);
        }

        fputs(line, fp);
    }

    char indices[32];

    for (std::vector<int>& face : this->faces) {
        line[0] = '\0'; // clear
        snprintf(line, sizeof(line), "f");
        for (int& fv : face) {
            indices[0] = '\0'; // clear
            snprintf(indices, sizeof(indices), " %i", fv+1); // Add +1 for goz -> obj conversion
            strcat(line, indices);
        }
        strcat(line, "\n");
        fputs(line, fp);
    }

    fclose(fp);

    LOG("Finished exporting obj file.");
}

void GoZ::writePly(std::string out_path) {

    LOG("Exporting ply file for ZBruush...");

    FILE* fp;
    fp = fopen(out_path.c_str(), "w");

    if (fp == NULL) {
        printf("%s file cannot be opened\n", out_path.c_str());
        exit(EXIT_FAILURE);
    }
  
    // Export Vertex positions
    size_t numVerts = this->vertices.size();
    size_t numFaces = this->faces.size();

    char header[512];
    snprintf(header, sizeof(header), "ply\n"
                                     "format ascii 1.0\n"
                                     "element vertex %i\n"
                                     "property float x\n"
                                     "property float y\n"
                                     "property float z\n"
                                     "property uchar red\n"
                                     "property uchar green\n"
                                     "property uchar blue\n"
                                     "property uchar alpha\n"
                                     "element face %i\n"
                                     "property list uchar uint vertex_indices\n"
                                     "end_header\n", (int)numVerts, (int)numFaces);

    fputs(header, fp);
    
    char line[128];
    
    // Vertices
    for (size_t i=0; i<numVerts; i++) {
        std::vector<float>& v = this->vertices[i];
        std::vector<float>& Cd = this->vertexColor[i];
        int r = (int)(255 * Cd[0]);
        int g = (int)(255 * Cd[1]);
        int b = (int)(255 * Cd[2]);
        line[0] = '\0'; // clear
        snprintf(line, sizeof(line), "%f %f %f %i %i %i 255\n", v[0], -v[1], -v[2], r, g, b); // ZBrush is flipped in y and z by default so negate them
        fputs(line, fp);
    }

    // Faces
    char indices[32];
    
    for (std::vector<int>& face : this->faces) {
        line[0] = '\0'; // clear
        size_t numFaceVerts = face.size();

        snprintf(line, sizeof(line), "%i", (int)numFaceVerts);

        for (int& fv : face) {
            indices[0] = '\0'; // clear
            snprintf(indices, sizeof(indices), " %i", fv);
            strcat(line, indices);
        }
        strcat(line, "\n");
        fputs(line, fp);
    }

    fclose(fp);

    LOG("Finished exporting ply file.");
}
