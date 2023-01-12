#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

class obj
{
    public :
    GLuint vao_id; 
    std::vector<GLfloat> vertex_buffer_data;

    obj(GLuint _vao_id, std::vector<GLfloat> _vertex_buffer_data) {
        vao_id = _vao_id; 
        vertex_buffer_data = _vertex_buffer_data;
    };
};

obj *setup_bunny(GLuint progId);
obj *setup_floor(uint progId); 