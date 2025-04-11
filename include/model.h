//
// Created by Trey Bertram on 2025-04-11.
//

#ifndef MODEL_H
#define MODEL_H
#include "shader.h"
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "util_funcs.h"
#include <filesystem>

class Model
{
public:
    Model(const char *path) { load_model(path); }
    void draw(Shader &shader) {
        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].draw(shader);
    }
private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;

    void load_model(std::string path) {
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        process_node(scene->mRootNode, scene);
    }
    void process_node(aiNode *node, const aiScene *scene) {
        // process all the node's meshes (if any)
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(process_mesh(mesh, scene));
        }
        // then do the same for each of its children
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            process_node(node->mChildren[i], scene);
        }
    }

    std::vector<Texture> load_material_textures(aiMaterial *mat, const aiTextureType type, const std::string& type_name) {
        std::vector<Texture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for(unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if(!skip)
            {   // if texture hasn't been loaded already, load it
                std::filesystem::path texturePath(str.C_Str());
                std::string filename = texturePath.filename().string();
                std::filesystem::path fullPath = std::filesystem::path(directory) / filename;

                Texture texture;
                texture.id = load_texture(fullPath.string().c_str()); //  Use new relative fullPath when loading.
                texture.type = type_name;
                texture.path = filename;              //  Store just the filename (relative path).
                textures.push_back(texture);
                textures_loaded.push_back(texture); // add to loaded textures
            }
        }
        return textures;
    }

    Mesh process_mesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            // process vertex positions, normals and texture coordinates
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;

            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;

            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.tex_coords = vec;
            }
            else
                vertex.tex_coords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // process indices
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process material
        if(mesh->mMaterialIndex >= 0)
        {
            if(mesh->mMaterialIndex >= 0)
            {
                aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
                std::vector<Texture> diffuseMaps = load_material_textures(material,
                                                    aiTextureType_DIFFUSE, "texture_diffuse");
                textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
                std::vector<Texture> specularMaps = load_material_textures(material,
                                                    aiTextureType_SPECULAR, "texture_specular");
                textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            }
        }

        return Mesh(vertices, indices, textures);
    }

};

#endif //MODEL_H
