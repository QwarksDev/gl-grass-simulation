#pragma once
#define GLFW_INCLUDE_NONE
#include "camera.hh"
#include <GLFW/glfw3.h>
#include "obj.hh"

class Time {
public:
  static float inline last_frame = 0;
  static float inline time_passed = 0;
  static float inline delta_time = 0;

  static float deltaTime() {
    float currentFrame = glfwGetTime();
    delta_time = currentFrame - last_frame;
    last_frame = currentFrame;
    return delta_time;
  }


  static float update_time_passed() {
    time_passed += deltaTime();
    return time_passed;
  }

  static float get_time_passed() {
    return time_passed;
  }
};

void process_input(GLFWwindow *window, Camera *camera, obj* object_selected);