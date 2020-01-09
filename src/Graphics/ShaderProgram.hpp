#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Shader.hpp"

namespace Graphics {
  class ShaderProgram {
    unsigned _id;
    std::vector<Shader> _shaders{};

  public:
    ShaderProgram(const std::vector<Shader>& shaders);

    void Use() const;

    void SetUniform3f(const std::string& name, const glm::vec3 value) const;
    void SetUniformMat4f(const std::string& name, const glm::mat4& value) const;
  };
}