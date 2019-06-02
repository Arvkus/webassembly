#version 300 es
// adds the code to the top of all shader before they get sent to the GPU
// required to work for both GL and GLES
#ifdef GL_ES 
precision mediump float; 
precision mediump int;
#endif

out vec4 FragColor; 

in vec2 texture_pos;
in vec3 normal;
in vec3 vertex_pos;

uniform sampler2D texture_image;

void main()
{   

    vec3 light_color = vec3(0.9);
    vec3 object_color = vec3(0.9);
    vec3 ambient_color = vec3(0.6);

    // light calculations
    //vec3 norm = vec3(sin(ourNormal.x),sin(ourNormal.y),sin(ourNormal.z));

    vec3 light_pos = vec3(30, 20, 10);
    vec3 light_dir = normalize(light_pos - vertex_pos);

    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * light_color;

    vec3 result = (ambient_color + diffuse) * object_color;

    FragColor = texture(texture_image, texture_pos) * vec4(result, 1.0);
    //FragColor = vec4(result, 1.0);
    //FragColor = vec4(.5,.5,.5,1.0f);
}