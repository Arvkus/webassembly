#version 300 es
layout (location = 0) in vec3 aPos;   
layout (location = 1) in vec3 aNormal;  
layout (location = 2) in vec3 aTexture;  

out vec3 ourNormal;
out vec3 frag_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    ourNormal = aNormal;
    frag_pos = vec3(model * vec4(aPos, 1.0)); // model position in space
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
}  

