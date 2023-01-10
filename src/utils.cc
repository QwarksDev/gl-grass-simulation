#include "utils.hh"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>

void check_gl_error(int line, const char* file) {
    GLenum err = glGetError();
    int iter = 0;
    while (err != GL_NO_ERROR && iter < 5) 
    {                                                                      
        std::cerr << "OpenGL ERROR! " << file << ":" << line << "- Code: " << err << std::endl; 
        err = glGetError();   
        iter++; 
    }
}

std::string load(const std::string &filename)
{
    std::ifstream input_src_file(filename, std::ios::in);
    std::string ligne;
    std::string file_content = "";
    if (input_src_file.fail())
    {
        std::cerr << "FAIL\n";
        return "";
    }

    while (getline(input_src_file, ligne))
    {
        file_content = file_content + ligne + "\n";
    }

    file_content += '\0';
    input_src_file.close();
    return file_content;
}