//define the function's prototypes ???
//#define GL3_PROTOTYPES 1
//#define GL_GLEXT_PROTOTYPES 1 //compiler will asume that some funcs are available
//#include <SDL/SDL_opengles2.h> // SDL GLES2 ??

#define GL3_PROTOTYPES 1
//#define EMSCRIPTEN

#include <GLES3/gl3.h>
#include <SDL/SDL.h>
#include "OL/shaders.hpp"
#include "OL/canvas.hpp"

#include <stdio.h>
#include <iostream>
#include <functional>

#ifdef EMSCRIPTEN

#include <unistd.h> // usleep function for loop
#include "emscripten/emscripten.h"

#else


#endif

void main_loop(){ loop(); }

int main(int argc, char *argv[]) {

    Canvas canvas = Canvas();
    SDL_Window *window = canvas.get_window(); // create window
    SDL_Event event; // track events

    Shader_program sp = Shader_program();

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };    

    unsigned int VAO;
    glGenBuffers(1, &VAO);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    //glGenVertexArrays(1, &VAO);
    //glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0 );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    std::function<void()> loop = [&]
    {
        // input
    
        // render
        glClearColor(0.12 ,0.1, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        sp.use();
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        SDL_GL_SwapWindow(window);
    };


    // main loop
    #ifdef EMSCRIPTEN

        emscripten_set_main_loop(loop, 0, true);

    #else

        while(true){
            loop();
            
            SDL_PollEvent(&event);
            canvas.handle_window_event(event);
            if(event.type == SDL_QUIT) break;
        }

    #endif
    
    return 0;   
}