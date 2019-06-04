#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <GLM/glm.hpp>
#include <GLES3/gl3.h>
#include <OL/loader.hpp>
#include <OL/vertex.hpp>

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

class Model{
private:

    // render data
    std::vector <Vertex> vertices;
    std::vector <unsigned int> indices; 
    
    std::string path;

    //buffers:
    // Vertex buffer object
    // Element buffer object
    // Vertex array object
    // Texture object
    unsigned int VBO, EBO, VAO, TO;

    // methods
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
        tris = indices.size()/3;
    }

    unsigned int get_vert_size_bytes(){
        return vert * sizeof(Vertex);
    }

    void bind_buffers(){

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // vertices
        glBindVertexArray(VAO);
        glBufferData(GL_ARRAY_BUFFER, get_vert_size_bytes(), &vertices[0] , GL_STATIC_DRAW);

        // attributes (how to read vbo)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0 ); // position
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)) ); // uvs
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(5*sizeof(float)) ); // normals
        glEnableVertexAttribArray(2);

    }

public:
    // properties
    glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
    float length = 10;
    unsigned int vert = 0;
    unsigned int tris = 0;

    void draw(){ 
        // out of bounds error/warning in browser, if failed to open file
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vert);
    }

    void initialize(unsigned int *sp){
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glGenTextures(1, &TO); 

        bool tex = Loader::read_texture_file(path + ".png", TO);
        bool obj = Loader::read_object_file(path + ".obj", tex, vertices, indices);
        
        if(obj){
            std::cout<< "OBJECT LOAD SUCCESS: " << path << std::endl;
            calculate_properties();
            bind_buffers();
            model_complexity(vert);
        }else{
            std::cout<< "OBJECT LOAD FAIL: " << path << std::endl;
        }


        unsigned int location = glGetUniformLocation(*sp, "tex");
        if(tex){
            glUniform1i(location, 1);
        }else{
            glUniform1i(location, 0);
        }
    }

    void desroy(){
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteTextures(1, &TO); 

        indices.clear();
        vertices.clear();
    }

    // constructor:
    Model(std::string p){ path = p;}

};