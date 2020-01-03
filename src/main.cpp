#include <cstdio>

#include "Graphics/Window.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/ShaderProgram.hpp"

using namespace sge;

int main() {
  Window window{640, 480, "Simple GUI Engine"};

  const auto vertex_shader = Graphics::Shader(GL_VERTEX_SHADER, "../src/Shaders/flat_color.vert");
  const auto fragment_shader = Graphics::Shader(GL_FRAGMENT_SHADER, "../src/Shaders/flat_color.frag");
  const auto shader_program = Graphics::ShaderProgram({vertex_shader, fragment_shader});

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

    shader_program.Use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    window.Update();
  }

  return 0;
}