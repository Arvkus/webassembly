/* Arvydas Vitkus PS-16 */

//define the function's prototypes
//#define GL3_PROTOTYPES 1
//#define GL_GLEXT_PROTOTYPES 1 //compiler will asume that some functions are available
//#include <SDL/SDL_opengles2.h> // SDL2 header

#include <GLES3/gl3.h>
#include <SDL/SDL.h>

#include "OL/canvas.hpp"
#include "OL/camera.hpp"

#include "OL/scene/shaders.hpp"
#include "OL/scene/model.hpp"


#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <stdio.h>
#include <iostream>
#include <functional>
#include <sstream>
#include <chrono>

std::function<void(int)> model_update;
std::function<void()> loop;
void main_loop(){ loop(); }

#ifdef EMSCRIPTEN
    #include <unistd.h> // usleep function for loop
    #include "emscripten/emscripten.h"

    extern "C"{
        EMSCRIPTEN_KEEPALIVE void load_model(int id){
            model_update(id);
        }
    }

    EM_JS(void,display_fps,(int f),{
        document.querySelector("#span_fps").innerHTML = "Kadrai: " + f;
    })
#else
    void display_fps(int f){
        //std::cout<<"Current fps: "<<f<<std::endl;
    }
#endif


int main(int argc, char *argv[]) {
    GLenum err;
    Canvas canvas = Canvas();
    Camera camera = Camera();

    SDL_Window *window = canvas.get_window(); // create window
    SDL_Event event; // track events

    Shader_program sp = Shader_program();

    //------------------------------------------------------------------------

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)640/480, 0.1f, 10000.0f);
    glm::mat4 view = camera.move(0,0);
    glm::mat4 model = glm::mat4(1.0f);

    //------------------------------------------------------------------------

    Model object = Model("filesystem/models/model1.obj");
    camera.origin = object.center;
    camera.distance = object.length*1.5;
    view = camera.zoom(0); // update camera position

    model_update = [&](int num){
        std::stringstream ss;

        ss << "filesystem/models/model" << num << ".obj";

        object = Model(ss.str());
        camera.origin = object.center;
        camera.distance = object.length*1.5;
        view = camera.zoom(0); // update camera position 
    };
    

    //------------------------------------------------------------------------
    bool mouse_hold = false;
    int frame_counter = 0;
    

    std::chrono::time_point<std::chrono::system_clock> last_update;
    last_update = std::chrono::system_clock::now();

    if((err = glGetError()) != GL_NO_ERROR){
        std::cout<<"error 2: " << err << std::endl;
    }
    loop = [&]{ 

        // milli > micro > nano
        std::chrono::duration<float, std::milli> elapsed = std::chrono::system_clock::now() - last_update;
        if(elapsed.count() >= 1000.0f){
            last_update = std::chrono::system_clock::now();
            display_fps(frame_counter);
            frame_counter = 0;
        }

        frame_counter++;
        
        //  render // ----------------
        //glClearColor(0.12 ,0.1, 0.2, 1.0);
        glClearColor(1.0 ,1.0 , 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sp.use();
        object.draw();
        
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

            if(event.type == SDL_KEYDOWN){ // klaviaturos ivestis
                char key = event.key.keysym.sym;
                if(key >= '0' && key <= '9'){
                    model_update((int)(key - 48));
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
        emscripten_set_main_loop(main_loop, 1000, true);
    #else
        while(true){
            loop();
            if(event.type == SDL_QUIT) break;
        }
    #endif
    
    return 0;   
}