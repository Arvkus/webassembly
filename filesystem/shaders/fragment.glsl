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

uniform int tex;
uniform sampler2D texture_image;

void main()
{   

    vec3 light_color = vec3(0.9);
    vec3 object_color = vec3(0.5);
    vec3 ambient_color = vec3(0.6);
 
    vec3 light_dir = normalize( vec3(-1,1,1) );
    vec3 norm_dir = normalize(normal);

    float angle = dot(norm_dir, light_dir);
    float brigtness = max(angle, 0.0);

    vec3 diffuse = brigtness * light_color;

    vec3 result = (diffuse + ambient_color) * object_color;

    //vec3 result = vec3(.5);
    if(tex == 1){
        FragColor = texture(texture_image, texture_pos) * vec4(result, 1.0);
    }else{
        FragColor = vec4(result, 1.0);
    }
    
}