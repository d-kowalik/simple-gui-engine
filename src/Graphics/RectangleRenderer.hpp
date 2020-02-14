#pragma once

#include "ShaderProgram.hpp"

#include "Rectangle.hpp"

#include <glad/glad.h>

namespace sge::Graphics {
  class RectangleRenderer {
    static const int SIZE = 100000;

    unsigned _vao, _vbo, _ebo, _ivbo;
    Ref<ShaderProgram> _program;
    unsigned _pixelCount = 0;

    unsigned _indices[6] {
        0, 1, 3,
        1, 2, 3
    };
    glm::vec3 _vertices[4] = {
        {0.f, 1.f, 1.f},
        {1.f, 1.f, 1.f},
        {1.f, 0.f, 1.f},
        {0.f, 0.f, 1.f}
    };

    struct InstanceData
    {
      glm::vec3 color;
      glm::mat4 model;
    };
    InstanceData* _buffer = nullptr;
  public:
    explicit RectangleRenderer(const Ref<ShaderProgram> program);
    ~RectangleRenderer();

    void Begin() {
      glBindBuffer(GL_ARRAY_BUFFER, _ivbo);
      _buffer = static_cast<InstanceData*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
      _program->Use();
    }

    void End() const {
      glUnmapBuffer(GL_ARRAY_BUFFER);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Draw();
    void Add(const sge::Ref<sge::Graphics::Rectangle>& rectangle);
  };
}


