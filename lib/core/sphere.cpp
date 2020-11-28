#include "sphere.h"

Sphere::Sphere(std::vector<Texture> textures, float radius, unsigned int segments)
    : Mesh(Sphere::generateVertices(radius, segments), Sphere::generateIndices(segments), textures)
{
}

std::vector<Vertex> Sphere::generateVertices(float radius, unsigned int segments)
{
    std::vector<Vertex> v;

    const float PI = 3.14159265359;

    for (unsigned int y = 0; y <= segments; y++)
    {
        for (unsigned int x = 0; x <= segments; x++)
        {
            float xSegment = (float)x / segments;
            float ySegment = (float)y / segments;

            float xPos = std::cos(xSegment * 2.0 * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0 * PI) * std::sin(ySegment * PI);

            glm::vec3 position = glm::vec3(xPos, yPos, zPos) * radius;
            glm::vec2 uv = glm::vec2(xSegment, ySegment);
            glm::vec3 normal = glm::vec3(xPos, yPos, zPos);

            v.push_back({position, normal, uv, glm::vec3(0.0f)});
        }
    }

    return v;
}

std::vector<unsigned int> Sphere::generateIndices(unsigned int segments)
{
    std::vector<unsigned int> v;
    bool oddRow = false;

    for (unsigned int y = 0; y < segments; ++y)
    {
        if (!oddRow) // even rows: y == 0, y == 2; and so on
        {
            for (unsigned int x = 0; x <= segments; ++x)
            {
                v.push_back(y * (segments + 1) + x);
                v.push_back((y + 1) * (segments + 1) + x);
            }
        }
        else
        {
            for (int x = segments; x >= 0; --x)
            {
                v.push_back((y + 1) * (segments + 1) + x);
                v.push_back(y * (segments + 1) + x);
            }
        }
        oddRow = !oddRow;
    }

    return v;
}