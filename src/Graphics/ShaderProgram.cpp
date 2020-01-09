#include "ShaderProgram.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace sge {
  Graphics::ShaderProgram::ShaderProgram(const std::vector<Shader> &shaders) : _shaders(shaders) {
    _id = glCreateProgram();

    for (const auto &shader : shaders)
      glAttachShader(_id, shader.GetId());
    glLinkProgram(_id);
    int success = 0;
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (!success) {
      GLint logSize = 0;
      glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &logSize);
      char *info_log = new char[logSize];
      glGetProgramInfoLog(_id, logSize, &logSize, info_log);
      printf("%s\n", info_log);

      glDeleteProgram(_id);
      delete[] info_log;
    }
  }

  void Graphics::ShaderProgram::Use() const {
    glUseProgram(_id);
  }

  void Graphics::ShaderProgram::SetUniformMat4f(const std::string &name, const glm::mat4 &value) const {
    unsigned location = glGetUniformLocation(_id, name.data());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
  }

  void Graphics::ShaderProgram::SetUniform3f(const std::string &name, const glm::vec3 value) const {
    unsigned location = glGetUniformLocation(_id, name.data());
    glUniform3f(location, value.x, value.y, value.z);
  }
}