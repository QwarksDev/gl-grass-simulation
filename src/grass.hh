#pragma once

#include <glm/vec3.hpp>
#include <vector>
#include "program.hh"

class grass
{
public:
    glm::vec3 *positions;
    int size;
    GLuint vao_id;
    std::vector<GLfloat> vertex_buffer_data;
    std::vector<vec3> bezier_base1; 
    std::vector<vec3> bezier_base2;
    std::vector<vec3> bezier_middle;
    std::vector<vec3> bezier_end; 


    program *prog;

    grass(glm::vec3 start_corner, glm::vec3 end_corner, int x_density, int z_density, program *prog_)
    {
        size = x_density * z_density;
        prog = prog_;
        positions = generate_grass_positions(start_corner, end_corner, x_density, z_density);
        setup_geometry();
    }

    ~grass()
    {
        free(positions);
    }

    void init_shader(Camera *camera);
    void init_compute_shader(program* compute_prog);

private:
    glm::vec3 *generate_grass_positions(glm::vec3 start_corner, glm::vec3 end_corner, int x_density, int y_density);
    void setup_geometry();
};