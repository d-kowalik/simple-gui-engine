#include <cstdio>

#include "Graphics/Window.hpp"
#include "Graphics/Shader.hpp"

using namespace sge;

int main() {
  Window window{640, 480, "Simple GUI Engine"};

  Graphics::Shader vertex_shader = Graphics::Shader(GL_VERTEX_SHADER, "../src/Shaders/flat_color.vert");
  Graphics::Shader fragment_shader = Graphics::Shader(GL_FRAGMENT_SHADER, "../src/Shaders/flat_color.frag");

  unsigned shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader.GetId());
  glAttachShader(shader_program, fragment_shader.GetId());
  glLinkProgram(shader_program);
  int success = 0;
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success)
  {
    GLint logSize = 0;
    glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &logSize);
    char* info_log = new char[logSize];
    glGetProgramInfoLog(shader_program, logSize, &logSize, info_log);
    printf("%s\n", info_log);

    glDeleteProgram(shader_program);
    vertex_shader.Delete();
    fragment_shader.Delete();
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