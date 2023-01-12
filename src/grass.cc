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

void bind_buffers(GLuint id, GLuint buff_id, vector<vec3> datas)
{
    glBindBuffer(GL_ARRAY_BUFFER, buff_id);
    check_gl_error(__LINE__, __FILE__);
    glBufferData(GL_ARRAY_BUFFER, datas.size() * sizeof(float),
                 (GLfloat *)datas.data(), GL_STATIC_DRAW);
    check_gl_error(__LINE__, __FILE__);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buff_id);
    glBindBuffer(GL_ARRAY_BUFFER, buff_id);
    glVertexAttribPointer(id, 3, GL_FLOAT, GL_FALSE, 0, 0);
    check_gl_error(__LINE__, __FILE__);
    glEnableVertexAttribArray(id);
    check_gl_error(__LINE__, __FILE__);
}

void grass::setup_geometry()
{
    vertex_buffer_data = vector<GLfloat>();
    bezier_base1 = vector<vec3>();
    bezier_base2 = vector<vec3>();
    bezier_middle = vector<vec3>();
    bezier_end = vector<vec3>();
    for (int i = 0; i < size; i++)
    {
        vec3 position = positions[i];
        for (size_t v = 0; v < vertex_rectangle.size(); v += 3)
        {
            vertex_buffer_data.push_back(vertex_rectangle[v] + position.x);
            vertex_buffer_data.push_back(vertex_rectangle[v + 1] + position.y);
            vertex_buffer_data.push_back(vertex_rectangle[v + 2] + position.z);
        }

        for (int i = 0; i < 6; i++)
        {
            // Init bezier point
            bezier_base1.push_back(positions[i] + vec3(0.2, 0.0, 0.0));
            bezier_base2.push_back(positions[i] + vec3(-0.2, 0.0, 0.0));
            bezier_middle.push_back(positions[i] + vec3(0.0, 1.5, 0.0));
            bezier_end.push_back(positions[i] + vec3(0.0, 2.0, 0.0));
        }
    }
    std::vector<GLuint> vbos = vector<GLuint>();
    vbos.push_back(get_attrib_location(prog->get_program_id(), "position"));
    vbos.push_back(get_attrib_location(prog->get_program_id(), "bezier_base1"));
    vbos.push_back(get_attrib_location(prog->get_program_id(), "bezier_base2"));
    vbos.push_back(get_attrib_location(prog->get_program_id(), "bezier_middle"));
    vbos.push_back(get_attrib_location(prog->get_program_id(), "bezier_end"));

    glGenVertexArrays(1, &vao_id);
    check_gl_error(__LINE__, __FILE__);
    glBindVertexArray(vao_id);
    check_gl_error(__LINE__, __FILE__);

    // glGenBuffers(vbos.size(), vbos.data());
    GLuint vbos_ids[vbos.size()];
    glGenBuffers(vbos.size(), vbos_ids);

    { // Vertex location
        check_gl_error(__LINE__, __FILE__);
        glBindBuffer(GL_ARRAY_BUFFER, vbos_ids[0]);
        check_gl_error(__LINE__, __FILE__);
        glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(float),
                     vertex_buffer_data.data(), GL_STATIC_DRAW);
        check_gl_error(__LINE__, __FILE__);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbos_ids[0]);
        glBindBuffer(GL_ARRAY_BUFFER, vbos_ids[0]);
        glVertexAttribPointer(vbos[0], 3, GL_FLOAT, GL_FALSE, 0, 0);
        check_gl_error(__LINE__, __FILE__);
        glEnableVertexAttribArray(vbos[0]);
        check_gl_error(__LINE__, __FILE__);
    }

    bind_buffers(vbos[1], vbos_ids[1], bezier_base1);
    bind_buffers(vbos[2], vbos_ids[2], bezier_base2);
    bind_buffers(vbos[3], vbos_ids[3], bezier_middle);
    bind_buffers(vbos[4], vbos_ids[4], bezier_end);

    check_gl_error(__LINE__, __FILE__);
    glBindVertexArray(0);
}

void grass::init_shader(Camera *camera)
{
    init_view_projection(prog, camera->get_view());
    // prog->set_uniform_float("anim_time", Time::get_time_passed());
    //  prog->set_uniform_int("nb_grass", size);
    //  glUniform3fv(glGetUniformLocation(prog->get_program_id(), "pointPositions"), bezier_buffer.size() * 3,
    //  glm::value_ptr(bezier_buffer[0]));
    glBindVertexArray(vao_id);
    check_gl_error(__LINE__, __FILE__);
    glPatchParameteri(GL_PATCH_VERTICES, 6);
    check_gl_error(__LINE__, __FILE__);
    glDrawArrays(GL_PATCHES, 0, vertex_buffer_data.size());
    check_gl_error(__LINE__, __FILE__);
    //glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size());
    check_gl_error(__LINE__, __FILE__);
}

void grass::init_compute_shader(program *compute_prog)
{
    compute_prog->set_uniform_float("anim_time", Time::get_time_passed());
    check_gl_error(__LINE__, __FILE__);
    /*if (vbo_bezier != -1)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo_bezier);
        check_gl_error(__LINE__, __FILE__);
    }*/

    // glDispatchCompute(bezier_buffer.size() / 12, 1, 1);
}