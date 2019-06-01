#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <GLM/glm.hpp>

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

    void read_file(const char * path){

        std::ifstream file(path);
        std::string word;
        float a,b,c;

        if (!file.is_open()) return;

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
    }

    void calculate_center(){
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

    }


public:

    std::vector <unsigned int> index_vertices; 
    std::vector <unsigned int> index_uvs;
    std::vector <unsigned int> index_normals;

    std::vector <float> vertices;
    std::vector <float> uvs;
    std::vector <float> normals;

    unsigned int vert = 0;
    unsigned int tris = 0;

    glm::vec3 center = glm::vec3(0.0f);
    float length;

    Model(const char * path){
        read_file(path);

        vert = vertices.size()/3;
        tris = index_vertices.size()/3;

        calculate_center();
    };


    ~Model(){};

};


// https://stackoverflow.com/questions/47313403/passing-client-files-to-webassembly-from-the-front-end
// https://gamedev.stackexchange.com/questions/75989/what-is-the-correct-way-to-reset-and-load-new-data-into-gl-array-buffer