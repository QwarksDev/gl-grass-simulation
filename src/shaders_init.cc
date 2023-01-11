#include "shaders_init.hh"


void init_view_projection(program *program, glm::mat4 view)
{
    GLuint model_view_matrix = program->GetUniformLocation("model_view_matrix");

    glUniformMatrix4fv(model_view_matrix, 1, GL_FALSE, &view[0][0]);
    check_gl_error(__LINE__, __FILE__);

    glm::mat4 projection = glm::frustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0);

    GLuint projection_matrix = program->GetUniformLocation("projection_matrix");
    glUniformMatrix4fv(projection_matrix, 1, GL_FALSE,
                       glm::value_ptr(projection));
    check_gl_error(__LINE__, __FILE__);
}

void init_bunny_shader(program *program, Camera *camera) 
{
    init_view_projection(program, camera->get_view());
    program->bind_objects();
}

void init_grass_shader(program *program, Camera *camera)
{
    init_view_projection(program, camera->get_view());
    program->bind_objects();
}