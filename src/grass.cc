#include "grass.hh"
#include <cstdlib>
#include <stdlib.h>
#include "../objects/rectangle.hh"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shaders_init.hh"
#include "utils.hh"
#include "input.hh"

using namespace std;
using namespace glm;

vec3 *grass::generate_grass_positions(vec3 start_corner, vec3 end_corner, int x_density, int z_density)
{
    float x_distance = abs(start_corner.x - end_corner.x) / x_density;
    float z_distance = abs(start_corner.z - end_corner.z) / z_density;

    vec3 *grass_positions = (vec3 *)malloc(sizeof(vec3) * x_density * z_density);

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
    bezier_buffer = vector<vec3>();
    for (int i = 0; i < size; i++)
    {
        vec3 position = positions[i];
        for (size_t v = 0; v < vertex_rectangle.size(); v += 3)
        {
            vertex_buffer_data.push_back(vertex_rectangle[v] + position.x);
            vertex_buffer_data.push_back(vertex_rectangle[v + 1] + position.y);
            vertex_buffer_data.push_back(vertex_rectangle[v + 2] + position.z);
        }

        // Init bezier point
        bezier_buffer.push_back(positions[i] + vec3(0.2, 0.0, 0.0));
        bezier_buffer.push_back(positions[i] + vec3(-0.2, 0.0, 0.0));
        bezier_buffer.push_back(positions[i] + vec3(0.0, 1.5, 0.0));
        bezier_buffer.push_back(positions[i] + vec3(0.0, 2.0, 0.0));
    }

    int max_nb_vbo = 5;
    int nb_vbo = 2;
    GLuint vbo_ids[max_nb_vbo];
    GLint vertex_location = glGetAttribLocation(prog->get_program_id(), "position");
    vbo_ids[0] = vertex_location;
    if (vertex_location == -1)
    {
        check_gl_error(__LINE__, __FILE__);
        std::cerr << "Error init vertex location !" << std::endl;
        exit(1);
    }
    vbo_bezier = glGetAttribLocation(prog->get_program_id(), "bezier_points");
    vbo_ids[1] = vbo_bezier;
    if (vbo_bezier == -1)
    {
        nb_vbo--;
        check_gl_error(__LINE__, __FILE__);
        std::cerr << "Error init vbo bezier !" << std::endl;
        // exit(1);
    }

    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    glGenBuffers(nb_vbo, vbo_ids);
    { // Vertex location
        check_gl_error(__LINE__, __FILE__);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[0]);
        check_gl_error(__LINE__, __FILE__);
        glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(float),
                     vertex_buffer_data.data(), GL_STATIC_DRAW);
        check_gl_error(__LINE__, __FILE__);
        glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
        check_gl_error(__LINE__, __FILE__);
        glEnableVertexAttribArray(vertex_location);
        check_gl_error(__LINE__, __FILE__);
    }

    if (vbo_bezier != -1)
    { // Bezier location
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[1]);
        glBufferData(GL_ARRAY_BUFFER, bezier_buffer.size() * sizeof(float) * 3,
                     glm::value_ptr(bezier_buffer[0]), GL_STATIC_DRAW);
        glVertexAttribPointer(vbo_bezier, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(vbo_bezier);
    }

    check_gl_error(__LINE__, __FILE__);
    glBindVertexArray(0);
}

void grass::init_shader(Camera *camera)
{
    init_view_projection(prog, camera->get_view());
    // prog->set_uniform_int("nb_grass", size);
    // glUniform3fv(glGetUniformLocation(prog->get_program_id(), "pointPositions"), bezier_buffer.size() * 3,
    // glm::value_ptr(bezier_buffer[0]));
    glBindVertexArray(vao_id);
    check_gl_error(__LINE__, __FILE__);
    glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size());
    check_gl_error(__LINE__, __FILE__);
}

void grass::init_compute_shader()
{
    prog->set_uniform_float("anim_time", Time::get_time_passed());
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo_bezier);
    check_gl_error(__LINE__, __FILE__);

    glDispatchCompute(bezier_buffer.size() / 12, 1, 1);
}