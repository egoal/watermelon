#include "shader.h"
#include <glog/logging.h>

char infolog[512];

Shader::Shader(const std::string &vfile, const std::string &ffile) {
  std::ifstream vfin(vfile), ffin(ffile);
  std::stringstream vss, fss;
  vss << vfin.rdbuf();
  fss << ffin.rdbuf();

  const std::string vcode = vss.str();
  const std::string fcode = fss.str();

  const char *vcodestr = vcode.c_str();
  const char *fcodestr = fcode.c_str();

  int success{0};

  uint vid = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vid, 1, &vcodestr, nullptr);
  glCompileShader(vid);
  glGetShaderiv(vid, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vid, 512, nullptr, infolog);
    LOG(WARNING) << "failed to compile vertex shader (" << vfile
                 << "): " << infolog;
  }

  uint fid = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fid, 1, &fcodestr, nullptr);
  glCompileShader(fid);
  glGetShaderiv(fid, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fid, 512, nullptr, infolog);
    LOG(WARNING) << "failed to compile fragment shader (" << ffile
                 << "): " << infolog;
  }

  id_ = glCreateProgram();
  glAttachShader(id_, vid);
  glAttachShader(id_, fid);
  glLinkProgram(id_);

  glGetProgramiv(id_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id_, 512, nullptr, infolog);
    LOG(WARNING) << "program link failed: " << infolog;
  }

  glDeleteShader(vid);
  glDeleteShader(fid);
}

void Shader::Use() const { glUseProgram(id_); }

void Shader::SetInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::SetVec3(const std::string &name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(id_, name.c_str()), x, y, z);
}
