#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "mesh.h"

class Sphere : public Mesh
{
private:
    float m_radius;

    std::vector<Vertex> generateVertices(float radius, unsigned int segments);
    std::vector<unsigned int> generateIndices(unsigned int segments);

public:
    Sphere(std::vector<Texture> textures, float radius = 1.0f, unsigned int segments = 64);
};
