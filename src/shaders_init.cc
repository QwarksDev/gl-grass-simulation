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

void init_simple_shaders(program *program, Camera *camera, glm::vec3 color, int force_color)
{
    program->set_uniform_vec3("color", color);
    program->set_uniform_int("unit_color", force_color);
    init_view_projection(program, camera->get_view());
    program->bind_objects();
    check_gl_error(__LINE__, __FILE__);
}

void init_sphere_shaders(program *program, Camera *camera, glm::vec3 color)
{
    program->set_uniform_float("scale", program->get_objects()[0]->scale);
    program->set_uniform_vec3("translation", program->get_objects()[0]->position);
    program->set_uniform_vec3("color", color);
    init_view_projection(program, camera->get_view());
    //init_simple_shaders(program, camera, color);

    glBindVertexArray(program->get_objects()[0]->vao_id);
    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, program->get_objects()[0]->vbo_indices);
    check_gl_error(__LINE__, __FILE__);

    // Draw the triangles !
    glDrawElements(
        GL_TRIANGLES,                              // mode
        program->get_objects()[0]->indices.size(), // count
        GL_UNSIGNED_INT,                           // type
        (void *)0                                  // element array buffer offset
    );
}