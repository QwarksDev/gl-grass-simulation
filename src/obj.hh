#pragma once
#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <vector>

class obj
{
    public :
    GLuint vao_id; 
    std::vector<GLfloat> vertex_buffer_data;
    glm::vec3 position; 
    float scale;
    GLuint vbo_indices;
    std::vector<uint> indices;

    obj(GLuint _vao_id, std::vector<GLfloat> _vertex_buffer_data) {
        vao_id = _vao_id; 
        vertex_buffer_data = _vertex_buffer_data;
    };
};

GLint get_attrib_location(int prog_id, const char *name);
void bind_buffers(GLuint id, GLuint buff_id, const GLfloat *datas, int size);
void bind_ssbo(GLuint buff_id, GLfloat *datas, int size, int location, GLenum type);
obj *setup_bunny(GLuint progId);
obj *setup_floor(uint progId); 
obj* setup_sphere(uint progId);