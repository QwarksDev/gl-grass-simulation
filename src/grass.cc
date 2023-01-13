#include "grass.hh"
#include <cstdlib>
#include <stdlib.h>
#include "../objects/rectangle.hh"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shaders_init.hh"
#include "utils.hh"
#include "input.hh"
#include "obj.hh"

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
            float offset_z = ((float(rand() % 1000) / 1000.0f) * 2.0f - 1.0f) * z_distance / 2.0f;
            float offset_x = ((float(rand() % 1000) / 1000.0f) * 2.0f - 1.0f) * x_distance / 2.0f;
            vec3 position = start_corner + vec3(x_distance * i + offset_x, start_corner.y, j * z_distance + offset_z);

            grass_positions[i * z_density + j] = position;
        }
    }
    return grass_positions;
}

void grass::setup_geometry(float min_height, float max_height)
{
    vertex_buffer_data = vector<GLfloat>();
    bezier_base1 = vector<vec3>();
    bezier_base2 = vector<vec3>();
    bezier_middle = vector<vec3>();
    bezier_end = vector<vec3>();
    heights = vector<float>();
    for (int i = 0; i < size; i++)
    {
        vec3 position = positions[i];

        float height = min_height + (float(rand() % 1000) / 1000.0f) * (max_height - min_height);
        heights.push_back(height);
        float rand_x_dir = ((float(rand() % 1000) / 1000.0f) * 2.0f - 1.0f);
        float rand_z_dir = ((float(rand() % 1000) / 1000.0f) * 2.0f - 1.0f);
        vec3 dir = normalize(vec3(rand_x_dir, 0.0f, rand_z_dir));

        for (size_t v = 0; v < vertex_rectangle.size(); v += 3)
        {
            vertex_buffer_data.push_back(vertex_rectangle[v] + position.x);
            vertex_buffer_data.push_back(vertex_rectangle[v + 1] + position.y);
            vertex_buffer_data.push_back(vertex_rectangle[v + 2] + position.z);

            // Init bezier point
            bezier_base1.push_back(position + dir * 0.01f);
            bezier_base2.push_back(position + -dir * 0.01f);
            bezier_middle.push_back(position + vec3(0.0f, 0.35f, 0.0f));
            bezier_end.push_back(position  + vec3(0.0f, height, 0.0f)) ;// dir * 0.2f;
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
    vbos_ids = (GLuint *)malloc(sizeof(GLuint) * vbos.size());
    glGenBuffers(vbos.size(), vbos_ids);

    bind_buffers(vbos[0], vbos_ids[0], vertex_buffer_data.data(), vertex_buffer_data.size() * sizeof(float));
    bind_buffers(vbos[1], vbos_ids[1], (GLfloat *)bezier_base1.data(), bezier_base1.size() * sizeof(float) * 3);
    bind_buffers(vbos[2], vbos_ids[2], (GLfloat *)bezier_base2.data(), bezier_base2.size() * sizeof(float) * 3);
    bind_buffers(vbos[3], vbos_ids[3], (GLfloat *)bezier_middle.data(), bezier_middle.size() * sizeof(float) * 3);
    bind_buffers(vbos[4], vbos_ids[4], (GLfloat *)bezier_end.data(), bezier_end.size() * sizeof(float) * 3);

    // Init ssbos
    bind_ssbo(vbos_ids[4], (GLfloat *)bezier_end.data(), bezier_end.size() * sizeof(float) * 3, 0, GL_SHADER_STORAGE_BUFFER);
    GLuint pos_vbo;
    glGenBuffers(1, &pos_vbo);
    bind_ssbo(pos_vbo, (GLfloat *)positions, size * sizeof(float) * 3, 1, GL_SHADER_STORAGE_BUFFER);
    GLuint height_vbo;
    glGenBuffers(1, &height_vbo);
    bind_ssbo(height_vbo, (GLfloat *)heights.data(), heights.size() * sizeof(float), 2, GL_SHADER_STORAGE_BUFFER);
    bind_ssbo(vbos_ids[3], (GLfloat *)bezier_middle.data(), bezier_middle.size() * sizeof(float) * 3, 3, GL_SHADER_STORAGE_BUFFER);

    check_gl_error(__LINE__, __FILE__);
    glBindVertexArray(0);
}

void grass::init_shader(Camera *camera)
{
    init_view_projection(prog, camera->get_view());
    prog->set_uniform_vec3("camera_position", camera->cameraPos);
    prog->set_uniform_float("render_distance", 4.0f);
    glBindVertexArray(vao_id);
    check_gl_error(__LINE__, __FILE__);
    glPatchParameteri(GL_PATCH_VERTICES, 6);
    check_gl_error(__LINE__, __FILE__);
    glDrawArrays(GL_PATCHES, 0, vertex_buffer_data.size());
    check_gl_error(__LINE__, __FILE__);
}

void grass::init_compute_shader(program *compute_prog, obj* sphere)
{
    compute_prog->set_uniform_float("anim_time", Time::get_time_passed());
    compute_prog->set_uniform_vec3("sphere_position", sphere->position);
    float radius = sphere->scale;
    compute_prog->set_uniform_float("sphere_radius", radius);

    check_gl_error(__LINE__, __FILE__);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbos_ids[4]);
    check_gl_error(__LINE__, __FILE__);

    glDispatchCompute(size * 6, 1, 1);
}