#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <GLM/glm.hpp>
#include <GLES3/gl3.h>

class Model{

private:
    void get_integers(std::string word, unsigned int *nums){ // get all integers from string (max 3)
        //std::cout<<" " << word <<" ";
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

    bool read_file(const char * path){

        std::ifstream file(path);
        std::string word;
        float a,b,c;

        if (!file.is_open()) return false;

        while(file >> word){
            
            if(word[0] == '#')file.ignore(1024,'\n'); // ignore until new line

            if(word == "v"){ // vertex
                file >> a >> b >> c;
                vertices.push_back(a);
                vertices.push_back(b);
                vertices.push_back(c);
                //std::cout<<a << " " <<b<< " "<< c << "" <<std::endl;
            }

            if(word == "vt"){ // texture coordinate
                file >> a >> b;
                uvs.push_back(a);
                uvs.push_back(b);
            }

            if(word == "vn"){ // texture coordinate
                file >> a >> b >> b;
                normals.push_back(a);
                normals.push_back(b);
                normals.push_back(c);
            }

            if(word == "f"){ // vertex, texture, normal indices
                for(int i = 0; i < 3; i++){
                    unsigned int* nums = new unsigned int[3];
                    file >> word;

                    get_integers(word, nums);
                    index_vertices.push_back( *(nums+0)-1 );
                    index_uvs.push_back( *(nums+1)-1 );
                    index_normals.push_back( *(nums+2)-1 );
                    //std::cout << *(nums+ 0) << " " << *(nums+ 1) << " "<< *(nums+ 2) << std::endl;

                }
            }

        }

        file.close();
        return true;
    }

    void calculate_properties(){
        glm::vec3 min(0.0f);
        glm::vec3 max(0.0f);
        glm::vec3 delta(0.0f);

        for(unsigned int i = 0; i < vertices.size(); i += 3){
            //std::cout << vertices[i+0] << " " << vertices[i+1] << " " << vertices[i+2] << std::endl;
            if(vertices[i+0] < min.x ) min.x = vertices[i+0];
            if(vertices[i+1] < min.y ) min.y = vertices[i+1];
            if(vertices[i+2] < min.z ) min.z = vertices[i+2];

            if(vertices[i+0] > max.x ) max.x = vertices[i+0];
            if(vertices[i+1] > max.y ) max.y = vertices[i+1];
            if(vertices[i+2] > max.z ) max.z = vertices[i+2];

        }
        center = (min + max) / (float)2; 

        delta = min - max;
        length = sqrt( pow(delta.x,2) + pow(delta.y,2) + pow(delta.z,2) );

        vert = vertices.size()/3; // x,y,z
        tris = index_vertices.size()/3; // x,y,z
    }

public:

    // compact vector data:
    std::vector <unsigned int> index_vertices; 
    std::vector <unsigned int> index_uvs;
    std::vector <unsigned int> index_normals;

    std::vector <float> vertices;
    std::vector <float> uvs;
    std::vector <float> normals;

    // metadata:
    unsigned int vert = 0; // vertices count
    unsigned int tris = 0; // triangles count

    glm::vec3 center = glm::vec3(0.0f); // center of model
    float length; // distance between furthest points

    // constructor:
    Model(const char * path){
        if( read_file(path)/* read file */){
            std::cout<< "File successful: " << path << std::endl;
            calculate_properties();
            std::cout<< "Vert: " << vert << ", Tris: " << tris << std::endl;


        }else{
            std::cout<< "Can't open file: " << path << std::endl;
        }

    };

    void draw(unsigned int &VAO){
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, index_vertices.size(), GL_UNSIGNED_INT, 0);
    }

    void bind_buffers(unsigned int &VBO, unsigned int &EBO, unsigned int &VAO){
        glBindVertexArray(VAO);

        // vertices of triangle
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) , &vertices[0], GL_STATIC_DRAW);

        // indices of triangle
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_vertices.size() * sizeof(unsigned int), &index_vertices[0], GL_STATIC_DRAW); 

        // attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0 );
        glEnableVertexAttribArray(0);

        //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3 * sizeof(float)));
        //glEnableVertexAttribArray(1);
    }

    ~Model(){};
};