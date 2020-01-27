#pragma once

#include "ShaderProgram.hpp"

#include "Rectangle.hpp"

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

    void Draw(const Rectangle& rectangle);
  };
}


