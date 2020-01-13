#include "RectangleRenderer.hpp"

#include <glad/glad.h>

sge::Graphics::RectangleRenderer::RectangleRenderer(const sge::Graphics::ShaderProgram &program) : _program(program) {
  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);

  glGenBuffers(1, &_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glGenBuffers(1, &_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void sge::Graphics::RectangleRenderer::Draw(glm::vec2 scale, glm::vec2 pos, glm::vec3 color) {
  float vertices[] = {
      pos.x, pos.y, .5f, color.x, color.y, color.z,
      pos.x, pos.y + scale.y, .5f, color.x, color.y, color.z,
      pos.x + scale.x, pos.y + scale.y, .5f, color.x, color.y, color.z,
      pos.x + scale.x, pos.y, .5f, color.x, color.y, color.z
  };

  glBindVertexArray(_vao);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);

  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

