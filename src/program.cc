#include "program.hh"

void program::add_shader(GLuint shd_id, GLenum type)
{
    shaders.push_back(shd_id);
    shaders_type.push_back(type);
}

GLuint program::load_shader(const std::string &src, GLenum type)
{
    objects_ = std::vector<obj *>();

    GLint compile_status = GL_TRUE;

    GLuint shader_id = glCreateShader(type);
    check_gl_error(__LINE__, __FILE__);

    glShaderSource(shader_id, 1, (const GLchar **)&(src), 0);
    check_gl_error(__LINE__, __FILE__);

    glCompileShader(shader_id);
    check_gl_error(__LINE__, __FILE__);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);
    check_gl_error(__LINE__, __FILE__);

    if (compile_status != GL_TRUE)
    {
        char *log = getlog(shader_id, GL_SHADER);
        if (log)
        {
            auto str = type == GL_VERTEX_SHADER ? " vertex " : "fragment ";
            std::cerr << "Shader " << str << shader_id << " : " << log
                      << std::endl;
            std::free(log);
        }

        glDeleteShader(shader_id);
        return -1;
    }

    return shader_id;
}

char *program::getlog(GLint id, GLenum type)
{
    GLint log_size;
    char *log;
    if (type == GL_SHADER)
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_size);
    else if (type == GL_PROGRAM)
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_size);
    else
        return nullptr;

    log = (char *)std::calloc(log_size + 1, 1);
    if (log != 0)
    {
        if (type == GL_SHADER)
            glGetShaderInfoLog(id, log_size, &log_size, log);
        else if (type == GL_PROGRAM)
            glGetProgramInfoLog(program_id_, log_size, &log_size, log);
        return log;
    }

    return nullptr;
}

void program::link_program()
{
    GLint link_status = GL_TRUE;
    program_id_ = glCreateProgram();
    check_gl_error(__LINE__, __FILE__);
    if (program_id_ == 0)
        return;

    for (auto shad : shaders)
    {
        glAttachShader(program_id_, shad);
        check_gl_error(__LINE__, __FILE__);
    }

    glLinkProgram(program_id_);
    check_gl_error(__LINE__, __FILE__);

    glGetProgramiv(program_id_, GL_LINK_STATUS, &link_status);
    check_gl_error(__LINE__, __FILE__);
    if (link_status != GL_TRUE)
    {
        char *program_log = getlog(program_id_, GL_PROGRAM);
        if (program_log)
        {
            std::cerr << "Program " << program_log << std::endl;
            std::free(program_log);
        }

        glDeleteProgram(program_id_);
        check_gl_error(__LINE__, __FILE__);

        for (size_t i = 0; i < shaders.size(); i++)
        {
            glDeleteShader(shaders[i]);
            check_gl_error(__LINE__, __FILE__);
        }

        program_id_ = 0;

        return;
    }

    ready_ = true;
}

void program::use()
{
    if (isready())
    {
        glUseProgram(program_id_);
        check_gl_error(__LINE__, __FILE__);
    }
    else
    {
        std::cerr << "Problem of init of program" << std::endl;
        check_gl_error(__LINE__, __FILE__);
    }
}

void program::bind_objects()
{
    for (size_t i = 0; i < objects_.size(); i++)
    {
        glBindVertexArray(objects_[i]->vao_id);
        check_gl_error(__LINE__, __FILE__);
        glDrawArrays(GL_TRIANGLES, 0, objects_[i]->vertex_buffer_data.size());
        check_gl_error(__LINE__, __FILE__);
    }
}

bool program::isready()
{
    return ready_;
}

GLint program::GetUniformLocation(const std::string &name) const
{
    GLint id = glGetUniformLocation(program_id_, name.c_str());
    check_gl_error(__LINE__, __FILE__);
    if (id == -1)
    {
        std::cout << "Uniform asked doesn't exist: " << name << " in program "
                  << program_id_ << std::endl;
    }
    return id;
}