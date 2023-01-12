#include <vector>
#include "obj.hh"
#include "../objects/bunny_dec.hh"
#include "../objects/floor.hh"
#include "utils.hh"


obj *setup_bunny(uint progId) 
{
    int max_nb_vbo = 5;
    int nb_vbo = 0;
    int index_vbo = 0;
    GLuint vbo_ids[max_nb_vbo];
    GLint vertex_location = glGetAttribLocation(progId, "position");
    check_gl_error(__LINE__, __FILE__);
    GLint normal_smooth_location = glGetAttribLocation(progId, "normalSmooth");
    check_gl_error(__LINE__, __FILE__);
    GLuint bunny_vao_id;
    glGenVertexArrays(1, &bunny_vao_id);
    check_gl_error(__LINE__, __FILE__);
    glBindVertexArray(bunny_vao_id);
    check_gl_error(__LINE__, __FILE__);
    if (vertex_location != -1)
        nb_vbo++;
    if (normal_smooth_location != -1)
        nb_vbo++;
    glGenBuffers(nb_vbo, vbo_ids);
    check_gl_error(__LINE__, __FILE__);
    if (vertex_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
        check_gl_error(__LINE__, __FILE__);
        glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(float),
                     vertex_buffer_data.data(), GL_STATIC_DRAW);
        check_gl_error(__LINE__, __FILE__);
        glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
        check_gl_error(__LINE__, __FILE__);
        glEnableVertexAttribArray(vertex_location);
        check_gl_error(__LINE__, __FILE__);
    }
    if (normal_smooth_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
        check_gl_error(__LINE__, __FILE__);
        glBufferData(GL_ARRAY_BUFFER, normal_smooth_buffer_data.size() * sizeof(float),
                     normal_smooth_buffer_data.data(), GL_STATIC_DRAW);
        check_gl_error(__LINE__, __FILE__);
        glVertexAttribPointer(normal_smooth_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
        check_gl_error(__LINE__, __FILE__);
        glEnableVertexAttribArray(normal_smooth_location);
        check_gl_error(__LINE__, __FILE__);
    }
    glBindVertexArray(0);
    return new obj(bunny_vao_id, vertex_buffer_data);
}


obj *setup_floor(uint progId) 
{

    GLuint vbo_id;
    GLint vertex_location = glGetAttribLocation(progId, "position");
    check_gl_error(__LINE__, __FILE__);
    GLuint vao_id;
    glGenVertexArrays(1, &vao_id);
    check_gl_error(__LINE__, __FILE__);
    glBindVertexArray(vao_id);
    check_gl_error(__LINE__, __FILE__);

    glGenBuffers(1, &vbo_id);
    check_gl_error(__LINE__, __FILE__);
    if (vertex_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        check_gl_error(__LINE__, __FILE__);
        glBufferData(GL_ARRAY_BUFFER, vertex_floor.size() * sizeof(float),
                     vertex_floor.data(), GL_STATIC_DRAW);
        check_gl_error(__LINE__, __FILE__);
        glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
        check_gl_error(__LINE__, __FILE__);
        glEnableVertexAttribArray(vertex_location);
        check_gl_error(__LINE__, __FILE__);
    }

    glBindVertexArray(0);
    return new obj(vao_id, vertex_buffer_data);
}