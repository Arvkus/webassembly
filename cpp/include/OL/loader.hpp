#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <GLM/glm.hpp>
#include <GLES3/gl3.h>
#include <OL/vertex.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class Loader{
private:

    // split "5/4/3" string to numbers 
    static void get_integers(std::string word, unsigned int *nums){ 

        std::string number;
        int len = word.length();
        int j = 0;

        for(int i = 0; i < len; i++){
            if(word[i] == '/'){
                
                *(nums + j) = std::atoi(number.c_str());
                number = "";

                j++;
                //if(++j == 3) j = 0;
            }else{
                number += word[i];
                if(i == len-1){ *(nums + j) = std::atoi(number.c_str()); }
            }
        }
    }



public:

    static bool read_object_file(
    std::string path, 
    bool texture_available,
    std::vector <Vertex> &vertices,
    std::vector <unsigned int> &indices
    ){
        std::cout<<"start a load:!!\n";
        // compact vector data:
        std::vector <glm::vec3> positions;
        std::vector <glm::vec2> uvs;
        std::vector <glm::vec3> normals;

        std::ifstream file(path);
        std::string word;
        float a,b,c;

        if (!file.is_open()) return false;

        while(file >> word){
            
            //if(word[0] == '#')file.ignore(1024,'\n'); // ignore until new line

            if(word == "v"){ // vertex coordinate
                file >> a >> b >> c;
                positions.push_back( glm::vec3(a,b,c) );
                //std::cout<<a << " " <<b<< " "<< c << "" <<std::endl;
            }

            if(word == "vt"){ // texture coordinate
                file >> a >> b;
                uvs.push_back( glm::vec2(a,b) );
            }

            if(word == "vn"){ // normal direction
                file >> a >> b >> b;
                normals.push_back( glm::vec3(a,b,c) );
            }

            if(word == "f"){ // vertex, texture, normal indices
            
                for(int i = 0; i < 3; i++){ // 3 vertex = triangle
                    unsigned int* nums = new unsigned int[3];
                    file >> word;
                    get_integers(word, nums); // "5/1/3" > 5,1,3

                    unsigned int index_position = *(nums+0)-1;
                    unsigned int index_uv       = *(nums+1)-1;
                    unsigned int index_normal   = *(nums+2)-1;

                    Vertex vertex; // model can have no texture coordinates
                    vertex.pos  = positions[index_position];
                    vertex.tex  = uvs.size() == 0? glm::vec2(0,0): uvs[index_uv]; 
                    vertex.norm = normals[index_normal];

                    vertices.push_back(vertex);
                    indices.push_back(index_position);

                }

            }
        }

        file.close();
        return true;
    }

    static bool read_texture_file(std::string path, unsigned int &TO){
        int width, height, nrChannels;
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

        if(height != width){

            return false;
        }
        
        bool size_ok = false;
        for(int i = 7; i < 13;i++){ // 128 - 4096
            if(width == pow(2,i)) size_ok = true;
        }

        if(!size_ok){
            return false;
        }
        
        if(data){
            glBindTexture(GL_TEXTURE_2D, TO);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            std::cout<<"TEXTURE LOAD SUCCESS: " << path << std::endl;
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }else{
            std::cout<<"TEXTURE LOAD FAIL [format?]: " << path << std::endl;
            return false;
        }
        stbi_image_free(data);
        return true;
    }

    
};
