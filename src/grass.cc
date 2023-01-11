#include "grass.hh"
#include <cstdlib>
#include <stdlib.h>
#include "../objects/rectangle.hh"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shaders_init.hh"
#include "utils.hh"

using namespace std; 
using namespace glm;

vec3* grass::generate_grass_positions(vec3 start_corner, vec3 end_corner, int x_density, int z_density)
{
    float x_distance = abs(start_corner.x - end_corner.x) / x_density;
    float z_distance = abs(start_corner.z - end_corner.z) / z_density;

    vec3 * grass_positions = (vec3 *)malloc(sizeof(vec3) * x_density * z_density);

    for (int i = 0; i < x_density; i++) 
    {
        for (int j = 0; j < z_density; j++)
        {
            vec3 position = start_corner + vec3(x_distance * i, start_corner.y, j * z_distance);
            grass_positions[i * z_density + j] = position;
        }
    }
    return grass_positions;
}

void grass::setup_geometry()
{
    vertex_buffer_data = vector<GLfloat>();
    for (int i = 0; i < size; i++) 
    {
        vec3 position = positions[i];
        for (size_t v = 0; v < vertex_rectangle.size(); v += 3) 
        {
            vertex_buffer_data.push_back(vertex_rectangle[v] + position.x);
            vertex_buffer_data.push_back(vertex_rectangle[v + 1] + position.y);
            vertex_buffer_data.push_back(vertex_rectangle[v + 2] + position.z);
        }
    }

    int max_nb_vbo = 5;
    int nb_vbo = 0;
    int index_vbo = 0;
    GLuint vbo_ids[max_nb_vbo];
    GLint vertex_location = glGetAttribLocation(prog->get_program_id(), "position");
    check_gl_error(__LINE__, __FILE__);

    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    if (vertex_location != -1)
        nb_vbo++;
    glGenBuffers(nb_vbo, vbo_ids);
    if (vertex_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
        glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(float),
                     vertex_buffer_data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(vertex_location);
    }
    check_gl_error(__LINE__, __FILE__);
    glBindVertexArray(0);
}

void grass::init_shader(Camera *camera)
{
    init_view_projection(prog, camera->get_view());
    glBindVertexArray(vao_id);
    check_gl_error(__LINE__, __FILE__);
    glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size());
    check_gl_error(__LINE__, __FILE__);
}