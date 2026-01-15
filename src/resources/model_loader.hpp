#pragma once

#include <string>
#include <vector>

struct MeshData
{
    // Placeholder; replace with real vertex/index buffers when integrating Assimp or custom importer.
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

class ModelLoader
{
public:
    // Returns true when a model is successfully loaded; stubbed for now.
    bool loadFromFile(const std::string & /*path*/)
    {
        return false;
    }
};
