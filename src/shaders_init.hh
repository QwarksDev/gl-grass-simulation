#pragma once

#include <GL/glew.h>
#include <glm/ext/vector_float4.hpp>
#include "program.hh"
#include "camera.hh"

void init_view_projection(program *program, glm::mat4 view);
void init_simple_shaders(program *program, Camera *camera, glm::vec3 color);