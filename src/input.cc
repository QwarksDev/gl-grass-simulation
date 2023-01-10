#include "input.hh"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include <iostream>

#include "camera.hh"

// Handle camera movement
void camera_moves(GLFWwindow *window, Camera *camera, float deltaTime)
{
    const float cameraSpeed = 10.0f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera->cameraPos += camera->objectForward * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera->cameraPos -= camera->objectForward * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera->cameraPos -= glm::normalize(glm::cross(camera->objectForward, glm::vec3(0, 1, 0))) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera->cameraPos += glm::normalize(glm::cross(camera->objectForward, glm::vec3(0, 1, 0))) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera->cameraPos += glm::vec3(0, 1, 0) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera->cameraPos -= glm::vec3(0, 1, 0) * cameraSpeed;

    }

    camera->update_view();

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        // Reset the camera
        camera->reset_cam();
        std::cout << "Press R - Reset the camera position and view angle" << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwDestroyWindow(window);
        std::cout << "Press Escape - Close window" << std::endl;
    }

}

float last_frame = 0;

void process_input(GLFWwindow *window, Camera *camera)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float currentFrame = glfwGetTime();
    float deltaTime = currentFrame - last_frame;
    last_frame = currentFrame;

    camera_moves(window, camera, deltaTime);
}
