#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <string>
#include <vector>

class program;

#include "utils.hh"
#include "obj.hh"

class program
{
private:
    GLuint program_id_;

    std::vector<GLuint> shaders;
    std::vector<GLenum> shaders_type;
    std::vector<obj *> objects_;

    bool ready_ = false;

public:
    GLuint values[12];
    program() = default;
    ~program() = default;
    char *getlog(GLint id, GLenum type);
    bool isready();
    void use();
    void add_shader(GLuint shd_id, GLenum type);

    GLuint load_shader(const std::string &, GLenum type);
    void link_program();
    GLint GetUniformLocation(const std::string &name) const;
    void set_texture_2D(const std::string &name, int index, int value_index);
    void bind_objects();

    inline GLuint get_program_id()
    {
        return program_id_;
    }

    inline std::vector<obj *> get_objects()
    {
        return objects_;
    }

    inline void add_object(obj *object)
    {
        objects_.push_back(object);
    }

    static program *make_program(const std::string shaders_path[], GLenum shaders_type[], int size)
    {
        program *prog = new program();

        for (int i = 0; i < size; i++)
        {
            auto shad_id = prog->load_shader(load(shaders_path[i]), shaders_type[i]);
            if (!shad_id)
            {
                std::cerr << "ERROR: Can't load shader: " << shaders_path[i] << std::endl;
                exit(1);
            }

            prog->add_shader(shad_id, shaders_type[i]);
        }

        prog->link_program();

        return prog;
    }

    inline void set_uniform_int(const std::string &name, int value) const
    {
        glUniform1i(GetUniformLocation(name), value);
    }

    inline void set_uniform_float(const std::string &name, float value) const
    {
        glUniform1f(GetUniformLocation(name), value);
    }

    inline void set_uniform_vec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(GetUniformLocation(name), 1, &value[0]);
    }

    inline void set_uniform_vec2(const std::string &name, float x, float y) const
    {
        glUniform2f(GetUniformLocation(name), x, y);
    }

    inline void set_uniform_vec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(GetUniformLocation(name), 1, &value[0]);
    }

    inline void set_uniform_vec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(GetUniformLocation(name), x, y, z);
    }

    inline void set_uniform_vec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(GetUniformLocation(name), 1, &value[0]);
    }

    inline void set_uniform_vec4(const std::string &name, float x, float y, float z, float w)
    {
        glUniform4f(GetUniformLocation(name), x, y, z, w);
    }
};
