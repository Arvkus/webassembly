#pragma once
#include <GLES3/gl3.h>
#include <SDL/SDL.h>
#include <stdio.h>

class Canvas{

private:
    int window_x = 640;
    int window_y = 480;

    SDL_Window *window;
    SDL_GLContext context;

public:
    Canvas(){

        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

        window = SDL_CreateWindow("Object Loader", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,window_x,window_y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if(!window) printf("Window was not created!");

        // set attributes
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        
        //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        //SDL_GL_SetSwapInterval(1); // turn on vsync

        context = SDL_GL_CreateContext(window); // openGL container/renderer

        glEnable(GL_DEPTH_TEST); 
        
        // successfuly created a window
        printf("Program start! | %s | %s\n",glGetString(GL_VERSION),glGetString(GL_RENDERER)); 


    }

    ~Canvas(){
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();

        printf("Program end! \n");
    }


    SDL_Window* get_window(){
        return window;
    }

    void set_window_size(int x, int y){
        SDL_SetWindowSize(window, x, y);
    }

    float get_aspect_ratio(){
        return (float)window_x/window_y;
    }

    void handle_window_event(SDL_Event event_object){

        if(event_object.type == SDL_WINDOWEVENT){
            if(event_object.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
                int x = event_object.window.data1;
                int y = event_object.window.data2;
                glViewport(0,0,x,y);
                std::cout<<" changed size"<<std::endl;
                window_x = x;
                window_y = y;
            }
        }
    }
    
};