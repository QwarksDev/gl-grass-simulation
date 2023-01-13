#include <vector>
#include "obj.hh"
#include "../objects/bunny_dec.hh"
#include "../objects/floor.hh"
#include "../objects/sphere.hh"
#include "utils.hh"
#include <iostream>
#include <cmath>

using namespace glm;

GLint get_attrib_location(int prog_id, const char *name)
{
    GLint vertex_location = glGetAttribLocation(prog_id, name);
    check_gl_error(__LINE__, __FILE__);
    if (vertex_location == -1)
    {
        std::cerr << "Error init " << name << " location !" << std::endl;
        exit(1);
    }
    return vertex_location;
}

void bind_buffers(GLuint id, GLuint buff_id, const GLfloat *datas, int size)
{
    glBindBuffer(GL_ARRAY_BUFFER, buff_id);
    check_gl_error(__LINE__, __FILE__);
    glBufferData(GL_ARRAY_BUFFER, size, datas, GL_STATIC_DRAW);
    check_gl_error(__LINE__, __FILE__);
    glVertexAttribPointer(id, 3, GL_FLOAT, GL_FALSE, 0, 0);
    check_gl_error(__LINE__, __FILE__);
    glEnableVertexAttribArray(id);
    check_gl_error(__LINE__, __FILE__);
}

void bind_ssbo(GLuint buff_id, GLfloat *datas, int size, int location, GLenum type)
{
    glBindBuffer(type, buff_id);
    check_gl_error(__LINE__, __FILE__);
    glBufferData(type, size, datas, GL_STATIC_DRAW);
    check_gl_error(__LINE__, __FILE__);
    glBindBufferBase(type, location, buff_id);
    check_gl_error(__LINE__, __FILE__);
}

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

    GLuint vbo_id[2];
    GLint vertex_location = get_attrib_location(progId, "position");
    GLint normal_location = get_attrib_location(progId, "normalSmooth");

    GLuint vao_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);
    check_gl_error(__LINE__, __FILE__);

    glGenBuffers(2, vbo_id);
    check_gl_error(__LINE__, __FILE__);

    bind_buffers(vertex_location, vbo_id[0], vertex_floor.data(), vertex_floor.size() * sizeof(float));
    bind_buffers(normal_location, vbo_id[1], normal_floor.data(), normal_floor.size() * sizeof(float));

    glBindVertexArray(0);
    return new obj(vao_id, vertex_buffer_data);
}

uint build_sphere(int horizontalSegments, int verticalSegments, std::vector<float> &vertex_data, std::vector<float> &normal_data, std::vector<unsigned int>& indices)
{
    float radius = 1;
    // const int vertexCount = (horizontalSegments + 1) * (verticalSegments + 1);

    vec3 vertexTmp = vec3(0.0, 0.0, 0.0);
    float pi = 3.1415926;
    int index = 0;
    for (int y = 0; y <= verticalSegments; ++y)
    {
        const float v = (float)y / (float)verticalSegments;
        const float theta = v * pi;

        for (int x = 0; x <= horizontalSegments; ++x)
        {
            const float u = (float)x / (float)horizontalSegments;
            vertexTmp = vec3(radius * cos(u * 2 * pi) * sin(theta), radius * cos(theta), radius * sin(u * pi * 2) * sin(theta));

            vertex_data.push_back(vertexTmp[0]);
            vertex_data.push_back(vertexTmp[1]);
            vertex_data.push_back(vertexTmp[2]);

            normal_data.push_back(vertexTmp[0]);
            normal_data.push_back(vertexTmp[1]);
            normal_data.push_back(vertexTmp[2]);
            ++index;
        }
    }

    for (int y = 0; y < verticalSegments; ++y)
    {
        if (y ==  0 || y % 2 == 0)
        {
            for (int x = 0; x <= horizontalSegments; ++x)
            {
                indices.push_back(y * (horizontalSegments + 1) + x);
                indices.push_back((y + 1) * (horizontalSegments + 1) + x);
            }
        }
        else
        {
            for (int x = horizontalSegments; x >= 0; --x)
            {
                indices.push_back((y + 1) * (horizontalSegments + 1) + x);
                indices.push_back(y * (horizontalSegments + 1) + x);
            }
        }
    }

    // Generate a buffer for the indices
    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    return elementbuffer;
}

obj *setup_sphere(uint progId)
{
    GLuint vbo_id[2];
    GLint vertex_location = get_attrib_location(progId, "position");
    GLint normal_location = get_attrib_location(progId, "normalSmooth");

    GLuint vao_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);
    check_gl_error(__LINE__, __FILE__);

    glGenBuffers(2, vbo_id);
    check_gl_error(__LINE__, __FILE__);

    std::vector<float> vertex_data = std::vector<float>();
    std::vector<float> normal_data = std::vector<float>();
    std::vector<unsigned int> indices = std::vector<unsigned int>();
    uint vbo_indices = build_sphere(10, 10, vertex_data, normal_data, indices);
    bind_buffers(vertex_location, vbo_id[0], vertex_data.data(), vertex_data.size() * sizeof(float));
    bind_buffers(normal_location, vbo_id[1], normal_data.data(), normal_data.size() * sizeof(float));

    glBindVertexArray(0);
    obj *res = new obj(vao_id, vertex_buffer_data);
    res->scale = 1.0f;
    res->position = glm::vec3(0.0f, 0.25f, 0.0f);
    res->indices = indices; 
    res->vbo_indices = vbo_indices;
    return res;
}