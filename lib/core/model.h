#pragma once

#include <string>
#include <vector>

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>

#include "shader.h"
#include "mesh.h"

class Model
{
public:
    Model(const char *path)
    {
        loadModel(path);
    }
    void draw(Shader &shader);

private:
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    std::string directory;

    void loadModel(const std::string path);
    void processNode(const aiNode *node, const aiScene *scene);
    Mesh processMesh(const aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};