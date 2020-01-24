#include "Shader.hpp"

#include <glad/glad.h>

#include "Util/FileUtil.hpp"

namespace sge::Graphics {
  Shader::Shader(unsigned type, const std::string &data) : _type(type) {
      Create(data);
    }


    void Shader::Delete() const {
      glDeleteShader(_id);
    }

    Shader::~Shader() {
      Delete();
    }

    void Shader::Create(const std::string &data) {
      const auto c_data = data.data();

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

  Shader* Shader::CreateFromRawData(unsigned type, const std::string &data) {
    return new Shader(type, data);
  }

  Shader *Shader::CreateFromPath(unsigned type, const std::string &path) {
    const auto data = sge::FileUtil::Read(path);
    const auto source = data.value_or("");

    return new Shader(type, source);
  }
}