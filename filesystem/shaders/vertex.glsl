#version 300 es
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texture;     
layout (location = 2) in vec3 a_normal;  


out vec3 normal;
out vec2 texture_pos;
out vec3 vertex_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    normal = a_normal;
    texture_pos = a_texture;

    vertex_pos = vec3(model * vec4(a_position, 1.0)); // vertex position in world space

    gl_Position = projection * view * model * vec4(a_position, 1.0);
    
}  

