// #include "glad/glad.h"
// #include "ll/ll.hpp"
#include <GLFW/glfw3.h>
#include <glog/logging.h>

constexpr float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

void CallbackFramebufferSize(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main(int, char **) {
  google::InitGoogleLogging("viewer");
  //   google::SetLogDestination(google::GLOG_INFO, "./");
  google::SetStderrLogging(google::GLOG_INFO);
  //   google::SetLogFilenameExtension(".log");

  LOG(INFO) << "1";

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
  if (!window) {
    LOG(WARNING) << "failed to create glfw window.";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  //   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
  //     LOG(WARNING) << "failed to initialize glad.";
  //     return -1;
  //   }

  glfwSetFramebufferSizeCallback(window, CallbackFramebufferSize);

  while (!glfwWindowShouldClose(window)) {
    ProcessInput(window);

    glClearColor(.2f, .3f, .3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
