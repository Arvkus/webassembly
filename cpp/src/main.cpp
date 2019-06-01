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
#include <sstream>

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
    // Vertex buffer object
    // Element buffer object
    // Vertex array object
    unsigned int VBO, EBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //------------------------------------------------------------------------

    Model object = Model("filesystem/models/model1.obj");
    object.bind_buffers(VBO, EBO, VAO);

    camera.origin = object.center;
    camera.distance = object.length*1.5;

    //------------------------------------------------------------------------

    bool mouse_hold = false;

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)640/480, 0.1f, 100.0f);
    glm::mat4 view = camera.move(0,0);
    glm::mat4 model = glm::mat4(1.0f);

    //camera = glm::translate(camera, glm::vec3(0.0f, -1.5f, -5.0f));

    //------------------------------------------------------------------------

    loop = [&]
    {  
        //  render // ----------------
        sp.use();
        glClearColor(0.12 ,0.1, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        object.draw(VAO);

        // events // ----------------
        if(SDL_PollEvent(&event)){ // if there's an event

            // window event
            bool resized = canvas.handle_window_event(event);
            if(resized) projection = glm::perspective(glm::radians(45.0f), canvas.get_aspect_ratio(), 0.1f, 100.0f);

            // mouse events
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

            if(event.type == SDL_KEYDOWN){
                char key = event.key.keysym.sym;
                if(key >= '0' && key <= '9'){
                    std::cout << key << std::endl; 

                    std::stringstream ss;
                    ss << "filesystem/models/model" << key << ".obj";

                    object = Model(ss.str().c_str());
                    object.bind_buffers(VBO, EBO, VAO);

                    camera.origin = object.center;
                    camera.distance = object.length*1.5;
                    view = camera.zoom(0); // update camera position

                    

                }
                
            }

        }
        
        //swap buffer / set uniforms // ----------------
        sp.set_uniform("projection", projection);
        sp.set_uniform("view", view);   
        sp.set_uniform("model", model);

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