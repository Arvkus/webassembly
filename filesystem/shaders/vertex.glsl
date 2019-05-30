#version 300 es
layout (location = 0) in vec3 aPos;   
layout (location = 1) in vec2 aColor;
  
out vec3 ourColor; // output a color to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    ourColor = vec3(aColor, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
}  

