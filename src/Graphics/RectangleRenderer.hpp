#pragma once

#include "ShaderProgram.hpp"

namespace sge::Graphics {
  class RectangleRenderer {
    unsigned _vao, _vbo, _ebo;
    unsigned indices[6] = {
        0, 1, 3,
        1, 2, 3
    };
    ShaderProgram _program;

  public:
    explicit RectangleRenderer(const ShaderProgram& program);

    void Draw(glm::vec2 scale, glm::vec2 pos, glm::vec3 color);
  };
}


