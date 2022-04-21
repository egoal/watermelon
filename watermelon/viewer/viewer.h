#pragma once

#include "ll/ll.hpp"
#include <GLFW/glfw3.h>

class Shader;

class Viewer {
public:
  Viewer();
  ~Viewer();
  void Show();

private:
  GLFWwindow *window_;
  int winWidth_{800}, winHeight_{600};
  std::unique_ptr<Shader> shader_;

  GLFWwindow *CreateWindow(int winWidth, int winHeight,
                           const std::string &title) const;
  unsigned int LoadTexture(const std::string &imagefile) const;

  void Update(float dt);

  void ProcessInput(float dt);
};
