#version 300 es
layout (location = 0) in vec3 aPos;   
  
out vec3 ourColor; // output a color to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    ourColor = vec3( sin(aPos.x) , sin(aPos.y) , sin(aPos.z));
    //ourColor = vec3(1,1,1);

    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
}  

