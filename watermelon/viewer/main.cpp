#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "ll/ll.hpp"
#include "shader.h"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <glog/logging.h>

void CallbackFramebufferSize(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow *window, float dt, const Shader &shader) {
  static float alpha = 0.f;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    alpha = ll::clamp(alpha + 1.f * dt, 0.f, 1.f);
    shader.SetFloat("alpha", alpha);
  } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    alpha = ll::clamp(alpha - 1.f * dt, 0.f, 1.f);
    shader.SetFloat("alpha", alpha);
  }
}

unsigned int LoadTexture(const std::string &imagefile) {
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

int main(int, char **) {
  google::InitGoogleLogging("viewer");
  //   google::SetLogDestination(google::GLOG_INFO, "./");
  google::SetStderrLogging(google::GLOG_INFO);
  //   google::SetLogFilenameExtension(".log");

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  constexpr int screenWidth = 800;
  constexpr int screenHeight = 600;

  GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight,
                                        "LearnOpenGL", nullptr, nullptr);
  if (!window) {
    LOG(WARNING) << "failed to create glfw window.";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LOG(WARNING) << "failed to initialize glad.";
    return -1;
  }

  glfwSetFramebufferSizeCallback(window, CallbackFramebufferSize);

  // shader & program
  Shader shader("viewer/assets/shader.vs", "viewer/assets/shader.fs");

  // vertex
  // clang-format off
  constexpr float vertices[] = {
      .5f, .5f, .0f,    1.f, 0.f, 0.f,    1.f, 1.f,
      .5f, -.5f, .0f,   0.f, 1.f, 0.f,    1.f, 0.f,
      -.5f, -.5f, .0f,  0.f, 0.f, 1.f,    0.f, 0.f, 
      -.5f, .5f, .0f,   1.f, 1.f, 0.f,    0.f, 1.f

  };
  constexpr unsigned int indices[] = {
    0, 1, 3, 
    1, 2, 3
  };
  // clang-format on

  unsigned int texture0 = LoadTexture("viewer/assets/container.jpg");
  unsigned int texture1 = LoadTexture("viewer/assets/awesomeface.png");

  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0); // unbind

  shader.Use();
  shader.SetInt("texture1", 0);
  shader.SetInt("texture2", 1);
  shader.SetFloat("alpha", 0.2f);

  // glm::mat4 trans(1.f);
  // trans = glm::rotate(trans, glm::radians(45.f), glm::vec3(0.f, 0.f, 1.f));
  // trans = glm::scale(trans, glm::vec3(.5, .5, .5));
  // glUniformMatrix4fv(glGetUniformLocation(shader.ID(), "transform"), 1,
  //                    GL_FALSE, glm::value_ptr(trans));

  glm::mat4 model(1.f);
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  glm::mat4 view(1.f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

  glm::mat4 projection = glm::perspective(
      glm::radians(45.0f), screenWidth / static_cast<float>(screenHeight), 0.1f,
      100.0f);

  auto set_mat = [id = shader.ID()](const std::string &name,
                                    const glm::mat4 &mat) {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE,
                       glm::value_ptr(mat));
  };

  double lastTime = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    const double thistime = glfwGetTime();
    const double dt = thistime - lastTime;
    lastTime = thistime;

    ProcessInput(window, dt, shader);

    glClearColor(.2f, .3f, .3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // const float theta = glfwGetTime() * 2.0f;
    // float dx = std::cos(theta) * .25f;
    // float dy = std::sin(theta) * .25f;
    // shader.SetVec3("offset", dx, dy, .0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    shader.Use();

    set_mat("model", model);
    set_mat("view", view);
    set_mat("projection", projection);
    // glm::mat4 trans(1.f);
    // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(.0f,
    // .0f, 1.f)); trans = glm::translate(trans, glm::vec3(.5f, -.5f, .0f));
    // glUniformMatrix4fv(glGetUniformLocation(shader.ID(), "transform"), 1,
    //                    GL_FALSE, glm::value_ptr(trans));

    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  glfwTerminate();

  return 0;
}
