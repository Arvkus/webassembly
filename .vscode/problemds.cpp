
GLenum err;
if((err = glGetError()) != GL_NO_ERROR){
    std::cout<<"error: " << err << std::endl;
}else{
    std::cout<< "no error" << std::endl;
}

// linux = 0;
// web = 1281



