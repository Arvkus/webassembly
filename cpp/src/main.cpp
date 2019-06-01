//define the function's prototypes ???
//#define GL3_PROTOTYPES 1
//#define GL_GLEXT_PROTOTYPES 1 //compiler will asume that some funcs are available
//#include <SDL/SDL_opengles2.h> // SDL GLES2 ??

//#define GL3_PROTOTYPES 1
//#define EMSCRIPTEN

#include <GLES3/gl3.h>
#include <SDL/SDL.h>
#include "OL/shaders.hpp"
#include "OL/canvas.hpp"
#include "OL/camera.hpp"
#include "OL/model.hpp" 

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <stdio.h>
#include <iostream>
#include <functional>

std::function<void()> loop;
void main_loop(){ loop(); }

#ifdef EMSCRIPTEN
    #include <unistd.h> // usleep function for loop
    #include "emscripten/emscripten.h"

    extern "C"{
        EMSCRIPTEN_KEEPALIVE void change_ratio(int x, int y){
            std::cout<<"JS: " << x << " " << y << std::endl;
        }
    }
#else


#endif

void print(std::string a){
    std::cout<<a<<std::endl;
}

int main(int argc, char *argv[]) {

    Canvas canvas = Canvas();
    Camera camera = Camera();

    SDL_Window *window = canvas.get_window(); // create window
    SDL_Event event; // track events

    Shader_program sp = Shader_program();

    //------------------------------------------------------------------------

    Model object = Model("filesystem/models/windmill.obj");
    camera.origin = object.center;
    camera.distance = object.length*2;

    //------------------------------------------------------------------------

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) , vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, object.vertices.size() * sizeof(float) , &object.vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, object.index_vertices.size() * sizeof(unsigned int), &object.index_vertices[0], GL_STATIC_DRAW); 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0 );
    glEnableVertexAttribArray(0);

    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    
    //------------------------------------------------------------------------

    bool mouse_hold = false;

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)640/480, 0.1f, 100.0f);
    glm::mat4 view = camera.move(0,0);
    glm::mat4 model = glm::mat4(1.0f);
    //camera = glm::translate(camera, glm::vec3(0.0f, -1.5f, -5.0f));

    //------------------------------------------------------------------------

    loop = [&]
    {
        // input // ----------------
        sp.use();
        sp.set_uniform("projection", projection);
        sp.set_uniform("view", view);   
        sp.set_uniform("model", model);

        
        //  render // ----------------
        
        glClearColor(0.12 ,0.1, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, object.index_vertices.size(), GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, object.vertices.size()); 

        // events // ----------------
        if(SDL_PollEvent(&event)){ // if there's an event
            canvas.handle_window_event(event);
            projection = glm::perspective(glm::radians(45.0f), canvas.get_aspect_ratio(), 0.1f, 100.0f);

            if(event.type == SDL_MOUSEBUTTONDOWN){ mouse_hold = true;  }
            if(event.type == SDL_MOUSEBUTTONUP  ){ mouse_hold = false; }
            if(event.type == SDL_MOUSEMOTION && mouse_hold){
                int x = event.motion.xrel;
                int y = event.motion.yrel;
                
                view = camera.move(x, y);
                
            }
            if(event.type == SDL_MOUSEWHEEL){
                view = camera.zoom(event.wheel.y);
            }
        }
        
        //swap buffer // ----------------

        SDL_GL_SwapWindow(window);
    };


    // main loop
    #ifdef EMSCRIPTEN
        emscripten_set_main_loop(main_loop, 0, true);
    #else
        while(true){
            loop();
            if(event.type == SDL_QUIT) break;
        }
    #endif
    
    return 0;   
}