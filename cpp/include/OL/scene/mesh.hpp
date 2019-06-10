#pragma once
#include <vector>
#include <GLES3/gl3.h>

#include "OL/scene/vertex.hpp"
#include "OL/scene/shaders.hpp"

class Mesh{

private:
    unsigned int VBO, EBO, VAO;
    void setup(){
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);


         // vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0] , GL_STATIC_DRAW);

        // indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


        // attributes (how to read vbo) // offsetof macro TODO
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0 ); // position
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)) ); // uvs
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(5*sizeof(float)) ); // normals
        glEnableVertexAttribArray(2);

        //glBindVertexArray(0);
    }

public:
    // render data
    std::vector <Vertex> vertices;
    std::vector <unsigned int> indices; 
    std::vector <Texture> textures;

    // create mesh
    Mesh(std::vector<Vertex> v, std::vector<unsigned int> i, std::vector<Texture> t){
        this->vertices = v;
        this->indices  = i;
        this->textures = t;
        setup();
    }

    // draw mesh
    void draw(){
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void destroy(){
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

        indices.clear();
        vertices.clear();
        textures.clear();

        /*
        delete &VAO;
        delete &VBO;
        delete &EBO;

        delete &indices;
        delete &vertices;
        delete &textures;
        */

       VAO = 0;
       VBO = 0;
       EBO = 0;
    }
};