#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <GLM/glm.hpp>
#include <GLES3/gl3.h>

#ifdef EMSCRIPTEN
    #include "emscripten/emscripten.h"
    EM_JS(void,model_complexity,(int vert),{

        document.querySelector("#span_tris").innerHTML = "Trikapiai: " + vert/3;

    })
#else
    void model_complexity(int vert){
        std::cout<< "Vert: " << vert << std::endl;
    }
#endif

struct Vertex{
    glm::vec3 pos;
    glm::vec2 tex;
    glm::vec3 norm;

    Vertex(glm::vec3 p, glm::vec2 t, glm::vec3 n){
        pos = p;
        tex = t;
        norm = n;
    }
    Vertex(){};
};

class Model{
private:

    void get_integers(std::string word, unsigned int *nums){ // get all integers from string (max 3)

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
        
        // compact vector data:
        std::vector <glm::vec3> positions;
        std::vector <glm::vec2> uvs;
        std::vector <glm::vec3> normals;

        std::ifstream file(path);
        std::string word;
        float a,b,c;

        if (!file.is_open()) return false;

        while(file >> word){
            
            if(word[0] == '#')file.ignore(1024,'\n'); // ignore until new line

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

                    unsigned short index_position = *(nums+0)-1;
                    unsigned short index_uv       = *(nums+1)-1;
                    unsigned short index_normal   = *(nums+2)-1;

                    Vertex vertex;
                    vertex.pos  = positions[index_position];
                    vertex.tex  = uvs[index_uv]; // tex cord 1 - y (TODO)
                    vertex.norm = normals[index_normal];

                    vertices.push_back(vertex);
                }

            }
        }
        /*
        for(int i = 0; i < vertices_data.size();i++){
            std::cout<< vertices_data[i].pos[0] << " "
                     << vertices_data[i].pos[1] << " "
                     << vertices_data[i].pos[2] << " " <<std::endl;
        }
        */

        file.close();
        return true;
    }
    
    void calculate_properties(){
 
        glm::vec3 min(0.0f);
        glm::vec3 max(0.0f);
        glm::vec3 delta(0.0f);

        for(unsigned int i = 0; i < vertices.size(); i++){
            //std::cout << vertices[i+0] << " " << vertices[i+1] << " " << vertices[i+2] << std::endl;
            if(vertices[i].pos.x < min.x ) min.x = vertices[i].pos.x;
            if(vertices[i].pos.y < min.y ) min.y = vertices[i].pos.y;
            if(vertices[i].pos.z < min.z ) min.z = vertices[i].pos.z;

            if(vertices[i].pos.x > max.x ) max.x = vertices[i].pos.x;
            if(vertices[i].pos.y > max.y ) max.y = vertices[i].pos.y;
            if(vertices[i].pos.z > max.z ) max.z = vertices[i].pos.z;

        }
        center = (min + max) / (float)2; 

        delta = min - max;
        length = sqrt( pow(delta.x,2) + pow(delta.y,2) + pow(delta.z,2) );

        vert = vertices.size(); // x,y,z

    }

    unsigned int vertices_size(){ // bytes
        return vertices.size() * sizeof(Vertex);
    }
    
    std::vector <Vertex> vertices;

public:

    // properties:
    unsigned int vert = 0; // vertices count
    float length = 10; // distance between furthest points
    glm::vec3 center = glm::vec3(0.0f); // center of model
    
    // constructor:
    Model(const char * path){
        if( read_file(path)/* read file */){
            std::cout<< "File successful: " << path << std::endl;
            calculate_properties();
            
            //for(int i = 0; i < normals.size(); i++)normals[i] = 0.5f; // testing
            model_complexity(vert);
        }else{
            std::cout<< "Can't open file: " << path << std::endl;
            
            
        }

    };

    void draw(unsigned int &VAO){ 
        // out of bounds error/warning in browser, if failed to open file
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }


    void bind_buffers(unsigned int &VBO, unsigned int &EBO, unsigned int &VAO){
        //vertices_data;
        std::cout<<"Sizes: "<< vertices.size() << " " << sizeof(Vertex) << std::endl;
        

        glBindVertexArray(VAO);

        // vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices_size(), &vertices[0] , GL_DYNAMIC_DRAW);

        // attributes (how to read vbo)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0 ); // position
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)) ); // uvs
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(5*sizeof(float)) ); // normals
        glEnableVertexAttribArray(2);
 


    }

    /*
    ~Model(){
        std::cout<<"model destruction"<<std::endl;
        // deallocate memory ???
        glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	    glDeleteVertexArrays(1, &m_vertexArrayObject);
    };
    */
};