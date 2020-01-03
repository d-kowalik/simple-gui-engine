#include <cstdio>

#include "Graphics/Window.hpp"
#include "Util/FileUtil.hpp"

using namespace sge;

int main() {
  Window window{640, 480, "Simple GUI Engine"};


  const auto vert_data = sge::FileUtil::Read("../src/Shaders/flat_color.vert");
  const auto frag_data = sge::FileUtil::Read("../src/Shaders/flat_color.frag");
  const auto vert_source = vert_data.value_or("");
  const auto frag_source = frag_data.value_or("");
  const auto frag_c_data = frag_source.data();
  const auto vert_c_data = vert_source.data();

  unsigned shader_program, vertex_shader, fragment_shader;
  shader_program = glCreateProgram();
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vertex_shader, 1, &vert_c_data, nullptr);
  glCompileShader(vertex_shader);
  int success = 0;
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLint logSize = 0;
    glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &logSize);
    char* info_log = new char[logSize];
    glGetShaderInfoLog(vertex_shader, logSize, &logSize, info_log);
    printf("%s\n", info_log);
    glDeleteShader(vertex_shader);
    delete[] info_log;
  }

  glShaderSource(fragment_shader, 1, &frag_c_data, nullptr);
  glCompileShader(fragment_shader);
  success = 0;
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLint logSize = 0;
    glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &logSize);
    char* info_log = new char[logSize];
    glGetShaderInfoLog(fragment_shader, logSize, &logSize, info_log);
    printf("%s\n", info_log);
    glDeleteShader(fragment_shader);
    delete[] info_log;
  }

  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success)
  {
    GLint logSize = 0;
    glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &logSize);
    char* info_log = new char[logSize];
    glGetProgramInfoLog(shader_program, logSize, &logSize, info_log);
    printf("%s\n", info_log);

    glDeleteProgram(shader_program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    delete[] info_log;
  }

  float vertices[] = {
      -.5f, -.5f, 0.f, 1.f, .5f, 1.f,
      -.5f, .5f, 0.f, .5f, 1.f, 1.f,
      .5f, .5f, 0.f, 1.f, .5f, .5f,
      .5f, -.5f, 0.f, 1.f, 1.f, .5f
  };

  unsigned indices[] = {
      0, 1, 3,
      1, 2, 3
  };

  unsigned vao, vbo, ebo;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  while (!window.ShouldClose()) {
    window.Clear();

    glUseProgram(shader_program);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    window.Update();
  }

  return 0;
}