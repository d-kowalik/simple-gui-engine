#include "RectangleRenderer.hpp"

#include <glad/glad.h>

sge::Graphics::RectangleRenderer::RectangleRenderer(const Ref<sge::Graphics::ShaderProgram> program) : _program(program) {
  glGenVertexArrays(1, &_vao);
  glGenBuffers(1, &_vbo);
  glGenBuffers(1, &_ebo);
  glGenBuffers(1, &_ivbo);

  glBindVertexArray(_vao);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices),
               _indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), static_cast<void*>(nullptr));
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, _ivbo);
  glBufferData(GL_ARRAY_BUFFER, SIZE * sizeof(InstanceData), nullptr, GL_STATIC_DRAW);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                        reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                        reinterpret_cast<void*>(sizeof(InstanceData::color)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                        reinterpret_cast<void*>(sizeof(InstanceData::color) + sizeof(glm::vec4)));
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                        reinterpret_cast<void*>(sizeof(InstanceData::color) + (2 * sizeof(glm::vec4))));
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                        reinterpret_cast<void*>(sizeof(InstanceData::color) + (3 * sizeof(glm::vec4))));
  glEnableVertexAttribArray(5);

  glVertexAttribDivisor(1, 1);
  glVertexAttribDivisor(2, 1);
  glVertexAttribDivisor(3, 1);
  glVertexAttribDivisor(4, 1);
  glVertexAttribDivisor(5, 1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

sge::Graphics::RectangleRenderer::~RectangleRenderer() {
  glDeleteVertexArrays(1, &_vao);
  glDeleteBuffers(1, &_vbo);
  glDeleteBuffers(1, &_ebo);
}

void sge::Graphics::RectangleRenderer::Add(const sge::Ref<sge::Graphics::Rectangle>& rectangle) {
  if (_buffer == nullptr) printf("ROFL!\n");
  _buffer->color = rectangle->color;
  _buffer->model = rectangle->GetModel();
  _buffer++;
  _pixelCount++;
}

void sge::Graphics::RectangleRenderer::Draw() {
  glBindVertexArray(_vao);
  glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL, _pixelCount);
  glBindVertexArray(0);
  _pixelCount = 0;
}
