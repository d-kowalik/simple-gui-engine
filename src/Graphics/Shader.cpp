#include "Shader.hpp"

#include <glad/glad.h>

#include "Util/FileUtil.hpp"

namespace Graphics {
  Shader::Shader(unsigned type, const std::string &path) : _type(type) {
    const auto data = sge::FileUtil::Read(path);
    const auto source = data.value_or("");
    const auto c_data = source.data();

    _id = glCreateShader(_type);
    glShaderSource(_id, 1, &c_data, nullptr);
    glCompileShader(_id);
    int success = 0;
    glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
    if (!success) {
      GLint logSize = 0;
      glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &logSize);
      char *info_log = new char[logSize];
      glGetShaderInfoLog(_id, logSize, &logSize, info_log);
      printf("SHADER COMPILATION FAILED:\n%s\n", info_log);
      glDeleteShader(_id);
      delete[] info_log;
    }
  }

  void Shader::Delete() const {
    glDeleteShader(_id);
  }

  Shader::~Shader() {
    Delete();
  }
}