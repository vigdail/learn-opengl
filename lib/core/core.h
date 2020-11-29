#pragma once

#include <string>
#include <vector>

unsigned int loadTexture(const std::string path);
unsigned int loadSTexture(const std::string path);
unsigned int loadCubemap(std::vector<std::string> faces);
unsigned int loadHDRTexture(const std::string path);