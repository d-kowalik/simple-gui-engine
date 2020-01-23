#pragma once

#include "Font.hpp"
#include "ShaderProgram.hpp"

namespace sge::Graphics {
  class FontRenderer {
    Font _font;
    ShaderProgram _program;
    unsigned _vao, _vbo;

  public:
    FontRenderer(const Font& font, const ShaderProgram& program);

    void Render(const std::string& text, glm::vec2 position, float scale, glm::vec3 color);
    glm::vec2 CheckSize(const std::string& text, float scale);
    const Font& GetFont() const { return _font; }
  };
}