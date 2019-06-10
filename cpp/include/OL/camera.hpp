#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <math.h>
#include <iostream>
class Camera{

private:

    glm::mat4 change_position(){
        glm::vec3 pos = glm::vec3();
        pos.x = cos(glm::radians((float)yaw)) * cos(glm::radians((float)pitch));
        pos.y = sin(glm::radians((float)yaw));
        pos.z = cos(glm::radians((float)yaw)) * sin(glm::radians((float)pitch));

        return glm::lookAt(origin + pos * distance, origin, glm::vec3(0,1,0));;
    }

public:
    glm::vec3 origin = glm::vec3(0,5,0);

    float distance = 5.0f;
    int pitch = 0; // vertical
    int yaw   = 0; // horizontal
    int roll  = 0;

    //----------------------------------------------

    Camera(){};
    ~Camera(){};

    glm::mat4 move(int x, int y){
        pitch = (pitch + x ) % 360;
        yaw = (yaw + y);

        if(yaw >  80) yaw =  80;
        if(yaw < -80) yaw = -80;
        
        return change_position();
    }

    glm::mat4 zoom(int offset){
        #ifdef EMSCRIPTEN
            distance -= (float)offset/60;
        #else
            distance -= (float)offset/2;
        #endif
        
        return change_position();
    }



};