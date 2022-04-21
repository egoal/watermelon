#include "viewer.h"
#include <GLFW/glfw3.h>
#include <glog/logging.h>

int main(int, char **) {
  google::InitGoogleLogging("viewer");
  //   google::SetLogDestination(google::GLOG_INFO, "./");
  google::SetStderrLogging(google::GLOG_INFO);
  //   google::SetLogFilenameExtension(".log");

  glfwInit();

  Viewer viewer;
  viewer.Show();

  glfwTerminate();

  return 0;
}
