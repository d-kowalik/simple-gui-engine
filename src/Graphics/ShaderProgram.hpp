#pragma once

#include <vector>

#include "Shader.hpp"

namespace Graphics {
  class ShaderProgram {
    unsigned _id;
    std::vector<Shader> _shaders{};

  public:
    ShaderProgram(const std::vector<Shader>& shaders);

    void Use() const;
  };
}