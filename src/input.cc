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
    const float cameraSpeed = 1.0f * deltaTime; // adjust accordingly
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

void object_moves(GLFWwindow *window, obj* object, float deltaTime) 
{
    if (object == nullptr)
        return;

    const float object_speed = 1.0f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        object->position += vec3(0.0, 0.0, 1.0) *  object_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
         object->position -= vec3(0.0, 0.0, 1.0) *  object_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
         object->position -= vec3(1.0, 0.0, 0.0) *  object_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
         object->position += vec3(1.0, 0.0, 0.0) *  object_speed;
    }
    
}

float last_frame = 0;

void process_input(GLFWwindow *window, Camera *camera, obj* object_selected)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float deltaTime = Time::delta_time;

    camera_moves(window, camera, deltaTime);
    object_moves(window, object_selected,  deltaTime);
}
