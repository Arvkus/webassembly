#pragma once
#include <iostream>
#include <vector>
#include <GLM/glm.hpp>
#include <GLES3/gl3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "OL/scene/shaders.hpp"
#include "OL/scene/mesh.hpp"
#include "OL/scene/vertex.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#ifdef EMSCRIPTEN
#include "emscripten/emscripten.h"
EM_JS(void, model_complexity, (int tris), {
    document.querySelector("#span_tris").innerHTML = "Trikapiai: " + tris / 3;
})
#else
void model_complexity(int tris)
{
    std::cout << "Triangles: " << tris / 3 << std::endl;
}
#endif

class Model
{
private:
    std::vector<Mesh> meshes;
    std::string directory;

    void load_model(std::string path)
    {
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return;
        }
        std::cout << "SUCCESS::ASSIMP::" << path << std::endl;

        directory = path.substr(0, path.find_last_of('/'));
        process_node(scene->mRootNode, scene);
    }

    void process_node(aiNode *node, const aiScene *scene)
    {
        // each node possibly has more nodes or meshes

        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        { // process node meshes
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(process_mesh(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        { // process node's children
            process_node(node->mChildren[i], scene);
        }
    };

    Mesh process_mesh(aiMesh *mesh, const aiScene *scene)
    {

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;

            // position
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.pos = vector;

            // normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.norm = vector;

            // texture (check if exists)
            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.tex = vec;
            }
            else
            {
                vertex.tex = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }
        /*
        if(mesh->mMaterialIndex >= 0){
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            std::vector<Texture> diffuseMaps = load_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            std::vector<Texture> specularMaps = load_textures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }  
        */

        return Mesh(vertices, indices, textures);
    };

    // create texture struct
    std::vector<Texture> load_textures(aiMaterial *mat, aiTextureType type, std::string typeName)
    {
        std::vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            Texture texture;
            texture.id = texture_from_file(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
        }
        return textures;
    };

    // create openGL texture object
    unsigned int texture_from_file(const char *path, const std::string &directory, bool gamma = false)
    {
        std::string filename = std::string(path);
        filename = directory + '/' + filename;

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }

    // properties
    void calculate_properties()
    {
        glm::vec3 min(0.0f);
        glm::vec3 max(0.0f);
        glm::vec3 delta(0.0f);

        vert = 0;
        tris = 0;

        for (unsigned int j = 0; j < meshes.size(); j++)
        {
            std::vector<Vertex> vertices = meshes[j].vertices;
            std::vector<unsigned int> indices = meshes[j].indices;

            for (unsigned int i = 0; i < vertices.size(); i++)
            {
                //std::cout << vertices[i+0] << " " << vertices[i+1] << " " << vertices[i+2] << std::endl;
                if (vertices[i].pos.x < min.x)
                    min.x = vertices[i].pos.x;
                if (vertices[i].pos.y < min.y)
                    min.y = vertices[i].pos.y;
                if (vertices[i].pos.z < min.z)
                    min.z = vertices[i].pos.z;

                if (vertices[i].pos.x > max.x)
                    max.x = vertices[i].pos.x;
                if (vertices[i].pos.y > max.y)
                    max.y = vertices[i].pos.y;
                if (vertices[i].pos.z > max.z)
                    max.z = vertices[i].pos.z;
            }

            vert += vertices.size();
            tris += indices.size();
        }

        center = (min + max) / (float)2;
        delta = min - max;
        length = sqrt(pow(delta.x, 2) + pow(delta.y, 2) + pow(delta.z, 2));
    }

public:
    // properties
    glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
    float length = 10;
    unsigned int vert = 0;
    unsigned int tris = 0;

    void draw()
    { // draw all meshes
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].draw();
    }

    // constructor:
    Model(std::string path)
    {
        load_model(path);
        calculate_properties();
        model_complexity(tris);
    }

    // release memory
    Model* operator=(Model other) {
        if(this != &other) {
            destroy();

            // swap properties
            std::swap(meshes, other.meshes);
            std::swap(directory, other.directory);
            std::swap(center, other.center);
            std::swap(length, other.length);
            std::swap(vert, other.vert);
            std::swap(tris, other.tris);
        }
        return this;
    }

    void destroy(){
        for(unsigned int i; i < meshes.size(); i++) meshes[i].destroy();
        meshes.clear();
    }

    ~Model(){destroy();}


};