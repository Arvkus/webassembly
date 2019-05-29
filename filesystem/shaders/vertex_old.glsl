#version 100 es
layout (location = 0) in vec3 aPos;   
layout (location = 1) in vec3 aColor;
  
out vec3 ourColor; // output a color to the fragment shader

void main()
{
    ourColor = aColor;
    gl_Position = vec4(aPos, 1.0);
    
}  