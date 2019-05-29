#version 300 es
// adds the code to the top of all shader before they get sent to the GPU
// required to work for both GL and GLES
#ifdef GL_ES 
precision mediump float; 
precision mediump int;
#endif

out vec4 FragColor;  
in vec3 ourColor;
  
void main()
{
    FragColor = vec4(ourColor, 1.0);
}
