#pragma once

#include "camera.hh"
#include <GLFW/glfw3.h>

class Time {
public:
  static float inline last_frame = 0;
  static float inline time_passed = 0;

  static float deltaTime() {
    float currentFrame = glfwGetTime();
    float deltaTime = currentFrame - last_frame;
    last_frame = currentFrame;
    return deltaTime;
  }

  static float update_time_passed() {
    time_passed += deltaTime();
    return time_passed;
  }

  static float get_time_passed() {
    return time_passed;
  }
};

void process_input(GLFWwindow *window, Camera *camera);