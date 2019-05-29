#pragma once

#include <GLES3/gl3.h>
#include <SDL/SDL.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>


class Shader_program{

private:
    unsigned int program;

    void shader_source( unsigned int &shader, const char* path){
        std::string code;
        std::ifstream file(path);

        if(file.is_open()){

            std::stringstream stream;
            stream << file.rdbuf();
            code = stream.str();

            const char *shader_code = code.c_str();
            //std::cout<< shader_code << std::endl;
            glShaderSource(shader,1, &shader_code, NULL);

            file.close();
        }else{
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        //return code;
    }

    void check_errors(unsigned int object, std::string type){
        int success;
        char info[1024];

        if(type == "VERTEX" || type == "FRAGMENT"){
            glGetShaderiv(object, GL_COMPILE_STATUS, &success);
            if(success){
                std::cout<<"SHADER::"<<type<<"::COMPILE_SUCCESS"<<std::endl;
            }else{
                glGetShaderInfoLog(object, 1024, NULL, info);
                std::cout<<"SHADER::"<<type<<"::COMPILE_FAIL: \n"<< info <<std::endl;
            }

        }else if(type == "PROGRAM"){
            glGetProgramiv(object, GL_LINK_STATUS, &success);
            if(success){
                std::cout<<"PROGRAM::"<<type<<"::LINK_SUCCESS"<<std::endl;
            }else{
                glGetProgramInfoLog(object, 1024, NULL, info);
                std::cout<<"PROGRAM::"<<type<<"::LINK_FAIL: \n"<< info <<std::endl;
            }
        }
    }


public:
    Shader_program(){

        // read and compile shaders
        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);

        shader_source(vertex,"filesystem/shaders/vertex.glsl");
        glCompileShader(vertex);
        check_errors(vertex,"VERTEX");

        shader_source(fragment,"filesystem/shaders/fragment.glsl");
        glCompileShader(fragment);
        check_errors(fragment,"FRAGMENT");

        // attach to program
        program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);
        check_errors(program,"PROGRAM");

        //delete unnecesary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    ~Shader_program(){
        glDeleteProgram(program);
    }

    void use(){
        glUseProgram(program);
    }

    unsigned int* get_program(){
        return &program;
    }

    void set_uniform(const char* variable, float value){
        unsigned int location = glGetUniformLocation(program, variable);
        glUniform1f(location, value);
    }   

    void set_uniform(const char* variable, glm::mat4 value){
        unsigned int location = glGetUniformLocation(program, variable);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void set_uniform(const char* variable, glm::vec4 value){
        unsigned int location = glGetUniformLocation(program, variable);
        glUniform4f(location, value[0], value[1], value[2], value[3]);
    }

    void set_uniform(const char* variable, glm::vec2 value){
        unsigned int location = glGetUniformLocation(program, variable);
        glUniform2f(location, value[0], value[1]);
    }

};
