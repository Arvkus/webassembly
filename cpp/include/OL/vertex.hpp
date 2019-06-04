#pragma once
#include <GLM/glm.hpp>

struct Vertex{ // Vertex contains all vertex data
    glm::vec3 pos; // position coordinate
    glm::vec2 tex; // texture coordinate
    glm::vec3 norm; // normal vector

    Vertex(glm::vec3 p, glm::vec2 t, glm::vec3 n){
        pos = p;
        tex = t;
        norm = n;
    }
    Vertex(){};
};