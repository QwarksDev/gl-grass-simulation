#pragma once

#include <GL/glew.h>
#include <glm/ext/vector_float4.hpp>
#include "program.hh"
#include "camera.hh"

typedef void (*shader_func)(program *, Camera*);

void init_bunny_shader(program *program, Camera *camera);