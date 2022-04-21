#pragma once

#include "glad/glad.h"
#include "ll/ll.hpp"

class Shader {
public:
  Shader(const std::string &vfile, const std::string &ffile);

  void Use() const;

  unsigned int ID() const { return id_; }

  void SetInt(const std::string &name, int value) const;
  void SetFloat(const std::string &name, float value) const;
  void SetVec3(const std::string &name, float x, float y, float z) const;

private:
  unsigned int id_;
};
