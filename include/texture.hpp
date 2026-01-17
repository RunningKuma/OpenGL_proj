#pragma once

#include <filesystem>

unsigned int loadTexture2D(const std::filesystem::path &path);
unsigned int createTextureFromData(int width, int height, const unsigned char *data);
unsigned int loadSkyboxTexture(const std::filesystem::path &path);
