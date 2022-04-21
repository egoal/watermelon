#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "shader.h"
#include "stb_image.h"
#include "viewer.h"
#include <GLFW/glfw3.h>
#include <glog/logging.h>

void CallbackFramebufferSize(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// vertex
// clang-format off
constexpr float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

constexpr unsigned int indices[] = {
0, 1, 3, 
1, 2, 3
};
// clang-format on

Viewer::Viewer() {}
Viewer::~Viewer() {}

void Viewer::Show() {
  window_ = CreateWindow(winWidth_, winHeight_, "Learn OpenGL");
  if (!window_)
    return;

  uint VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
  //                       (void *)(3 * sizeof(float)));
  // glEnableVertexAttribArray(1);
  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
  //                       (void *)(6 * sizeof(float)));
  // glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0); // unbind

  shader_ = std::make_unique<Shader>("viewer/assets/shader.vs",
                                     "viewer/assets/shader.fs");

  unsigned int texture0 = LoadTexture("viewer/assets/container.jpg");
  unsigned int texture1 = LoadTexture("viewer/assets/awesomeface.png");
  shader_->Use();
  shader_->SetInt("texture1", 0);
  shader_->SetInt("texture2", 1);
  shader_->SetFloat("alpha", .2f);

  //   glm::mat4 model(1.f);
  //   glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(.0f, .0f,
  //   -3.f));
  glm::mat4 projection =
      glm::perspective(glm::radians(45.f),
                       winWidth_ / static_cast<float>(winHeight_), .1f, 100.f);
  shader_->SetMat4("projection", projection);

  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  double lastTime = glfwGetTime();
  while (!glfwWindowShouldClose(window_)) {
    const double thistime = glfwGetTime();
    const double dt = thistime - lastTime;
    lastTime = thistime;

    Update(dt);

    glClearColor(.2f, .3f, .3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glBindVertexArray(VAO);
    shader_->Use();
    for (std::size_t i = 0; i < 10; ++i) {
      glm::mat4 model = glm::translate(glm::mat4(1.f), cubePositions[i]);
      if (i % 3 == 0)
        model = glm::rotate(model, ll::pi<float> * (float)glfwGetTime(),
                            glm::vec3(1.f, .3f, .5f));

      shader_->SetMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

GLFWwindow *Viewer::CreateWindow(int winWidth, int winHeight,
                                 const std::string &title) const {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(winWidth, winHeight, title.c_str(), nullptr, nullptr);
  if (!window) {
    LOG(WARNING) << "failed to create glfw window.";
    return nullptr;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LOG(WARNING) << "failed to initialize glad.";
    return nullptr;
  }

  glfwSetFramebufferSizeCallback(window, CallbackFramebufferSize);

  glEnable(GL_DEPTH_TEST);

  return window;
}

unsigned int Viewer::LoadTexture(const std::string &imagefile) const {
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  int width, height, channel;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data =
      stbi_load(imagefile.c_str(), &width, &height, &channel, 0);
  if (data) {
    LOG(INFO) << "channel: " << channel;
    if (channel == 3)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, data);
    else if (channel == 4)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, data);
    else
      CHECK(false) << "unexpected channel: " << channel;

    glGenerateMipmap(GL_TEXTURE_2D);
  } else
    LOG(WARNING) << "failed to load image.";
  stbi_image_free(data);

  return texture;
}

void Viewer::Update(float dt) {
  ProcessInput(dt);

  shader_->Use();
  float radius = 10.0f;
  float camX = sin(glfwGetTime()) * radius;
  float camZ = cos(glfwGetTime()) * radius;
  glm::mat4 view =
      glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0),
                  glm::vec3(0.0, 1.0, 0.0));
  shader_->SetMat4("view", view);
}

void Viewer::ProcessInput(float dt) {
  if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window_, true);
}
