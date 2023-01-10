#define GLFW_INCLUDE_NONE
#include <cstddef>
#include <fstream>
#include <glm/ext/vector_float4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <iterator>
#include <ostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "src/utils.hh"
#include "src/program.hh"
#include "src/obj.hh"

#define HEIGHT 900
#define WIDTH 1400

std::vector<program *> programs;

void framebuffer_size_callback(__attribute__((unused)) GLFWwindow *window,
                               int width, int height)
{
    glViewport(0, 0, width, height);
}

GLFWwindow *init_glfw()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    check_gl_error(__LINE__, __FILE__);

    GLFWwindow *window =
        glfwCreateWindow(WIDTH, HEIGHT, "Grass simulation project !", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    check_gl_error(__LINE__, __FILE__);
    return window;
}

void init_glew()
{
    int val = glewInit();
    if (val != GLEW_OK)
    {
        std::cerr << "ERROR Failed init glew: " << val << std::endl;
        exit(1);
    }
}

void init_GL()
{
    glEnable(GL_DEPTH_TEST);
    check_gl_error(__LINE__, __FILE__);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    check_gl_error(__LINE__, __FILE__);

    // glEnable(GL_CULL_FACE);
    glClearColor(1.0, 0.0, 0.0, 1.0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
}

void init_shaders()
{
    const std::string shaders[] = {"shaders/vertex_simple.shd", "shaders/fragment.shd"};
    GLenum types[] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
    program *bunny_prog = program::make_program(shaders, types, 2);
    bunny_prog->add_object(setup_bunny(bunny_prog->get_program_id()));
    programs.push_back(bunny_prog);
}

int main()
{
    GLFWwindow *window = init_glfw();
    init_glew();
    init_GL();
    check_gl_error(__LINE__, __FILE__);
    init_shaders();
    if (programs.size() == 0)
    {
        check_gl_error(__LINE__, __FILE__);
        std::cerr << "Programs not initialized !" << std::endl;
        std::exit(1);
    }
    // init_object();

    /*Camera *camera = new Camera();

    Mouse::init_mouse(camera);
    glfwSetCursorPosCallback(window, Mouse::mouse_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);*/

    while (!glfwWindowShouldClose(window))
    {

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        programs[0]->use();
        programs[0]->bind_objects();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}