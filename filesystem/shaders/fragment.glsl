#version 300 es
// adds the code to the top of all shader before they get sent to the GPU
// required to work for both GL and GLES
#ifdef GL_ES 
precision mediump float; 
precision mediump int;
#endif

out vec4 FragColor;  
in vec3 ourNormal;
in vec3 frag_pos;
  
void main()
{   
    vec3 light_color = vec3(0.9);
    vec3 object_color = vec3(0.4);
    vec3 ambient_color = vec3(0.4);

    // light calculations
    
    vec3 norm = normalize(ourNormal);
    vec3 light_pos = vec3(-10,30,-30);
    vec3 light_dir = normalize(light_pos - frag_pos);

    
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light_color;


    vec3 result = (ambient_color + diffuse) * object_color;

    FragColor = vec4(result, 1.0);
}
