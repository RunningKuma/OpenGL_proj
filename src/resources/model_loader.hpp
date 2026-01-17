#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ModelLoader
{
public:
    struct LoadedData
    {
        std::vector<float> vertices;            // Interleaved: Pos(3), Normal(3), UV(2)
        std::vector<unsigned char> textureData; // RGBA data for material atlas
        int texWidth = 0;
        int texHeight = 1;
        bool success = false;
    };

    static LoadedData loadOBJ(const std::string &path)
    {
        LoadedData data;
        std::vector<glm::vec3> temp_positions;
        std::map<std::string, int> materialMap;
        std::vector<glm::vec3> materialColors;
        int currentMatIndex = -1;

        std::filesystem::path objPath(path);
        std::filesystem::path mtlPath = objPath.parent_path() / "obj.mtl"; // Default guess

        // 1. Parse MTL if exists
        std::ifstream mtlFile(mtlPath);
        if (mtlFile.is_open())
        {
            std::cout << "Found MTL file: " << mtlPath << std::endl;
            std::string line;
            while (std::getline(mtlFile, line))
            {
                std::istringstream s(line);
                std::string cmd;
                s >> cmd;
                if (cmd == "newmtl")
                {
                    std::string matName;
                    s >> matName;
                    materialColors.push_back(glm::vec3(1.0f)); // Default white
                    materialMap[matName] = static_cast<int>(materialColors.size()) - 1;
                }
                else if (cmd == "Kd")
                {
                    if (!materialColors.empty())
                    {
                        float r, g, b;
                        s >> r >> g >> b;
                        materialColors.back() = glm::vec3(r, g, b);
                    }
                }
            }
        }
        else
        {
            std::cout << "MTL file not found: " << mtlPath << std::endl;
            // Fallback if no MTL
            materialColors.push_back(glm::vec3(1.0f));
        }

        // Generate Texture Data (Palette)
        data.texWidth = static_cast<int>(materialColors.size());
        if (data.texWidth == 0)
        {
            data.texWidth = 1;
            materialColors.push_back(glm::vec3(1.0f));
        }

        data.textureData.reserve(data.texWidth * 4);
        for (const auto &col : materialColors)
        {
            data.textureData.push_back(static_cast<unsigned char>(col.r * 255));
            data.textureData.push_back(static_cast<unsigned char>(col.g * 255));
            data.textureData.push_back(static_cast<unsigned char>(col.b * 255));
            data.textureData.push_back(255); // Alpha
        }

        std::ifstream file(path);
        if (!file.is_open())
        {
            std::cerr << "Failed to open OBJ file: " << path << std::endl;
            return data;
        }

        std::string line;
        while (std::getline(file, line))
        {
            if (line.substr(0, 6) == "usemtl")
            {
                std::istringstream s(line.substr(6));
                std::string matName;
                s >> matName;
                if (materialMap.find(matName) != materialMap.end())
                {
                    currentMatIndex = materialMap[matName];
                }
                else
                {
                    currentMatIndex = 0; // Fallback
                }
            }
            else if (line.substr(0, 2) == "v ")
            {
                std::istringstream s(line.substr(2));
                glm::vec3 v;
                s >> v.x >> v.y >> v.z;
                temp_positions.push_back(v);
            }
            else if (line.substr(0, 2) == "f ")
            {
                std::istringstream s(line.substr(2));
                std::string segment;
                std::vector<unsigned int> vIndices;

                while (s >> segment)
                {
                    std::istringstream part(segment);
                    std::string vIdxStr;
                    std::getline(part, vIdxStr, '/');
                    if (!vIdxStr.empty())
                    {
                        vIndices.push_back(std::stoi(vIdxStr));
                    }
                }

                if (vIndices.size() >= 3)
                {
                    // Calculate UV based on material index
                    // Map [0, N-1] to [0.0, 1.0]
                    // Center of pixel i: (i + 0.5) / N
                    float u = 0.5f;
                    if (data.texWidth > 0 && currentMatIndex >= 0)
                    {
                        u = (static_cast<float>(currentMatIndex) + 0.5f) / static_cast<float>(data.texWidth);
                    }

                    for (size_t i = 1; i + 1 < vIndices.size(); ++i)
                    {
                        unsigned int idx0 = vIndices[0] - 1;
                        unsigned int idx1 = vIndices[i] - 1;
                        unsigned int idx2 = vIndices[i + 1] - 1;

                        if (idx0 >= temp_positions.size() || idx1 >= temp_positions.size() || idx2 >= temp_positions.size())
                            continue;

                        glm::vec3 p0 = temp_positions[idx0];
                        glm::vec3 p1 = temp_positions[idx1];
                        glm::vec3 p2 = temp_positions[idx2];

                        glm::vec3 normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));

                        // Vertex 0
                        data.vertices.insert(data.vertices.end(), {p0.x, p0.y, p0.z});
                        data.vertices.insert(data.vertices.end(), {normal.x, normal.y, normal.z});
                        data.vertices.insert(data.vertices.end(), {u, 0.5f});

                        // Vertex 1
                        data.vertices.insert(data.vertices.end(), {p1.x, p1.y, p1.z});
                        data.vertices.insert(data.vertices.end(), {normal.x, normal.y, normal.z});
                        data.vertices.insert(data.vertices.end(), {u, 0.5f});

                        // Vertex 2
                        data.vertices.insert(data.vertices.end(), {p2.x, p2.y, p2.z});
                        data.vertices.insert(data.vertices.end(), {normal.x, normal.y, normal.z});
                        data.vertices.insert(data.vertices.end(), {u, 0.5f});
                    }
                }
            }
        }

        data.success = true;
        std::cout << "Loaded OBJ: " << path << " Verts: " << (data.vertices.size() / 8) << " Materials: " << data.texWidth << std::endl;
        return data;
    }
};
