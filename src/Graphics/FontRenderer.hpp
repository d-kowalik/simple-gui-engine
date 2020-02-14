#pragma once

#include "Font.hpp"
#include "ShaderProgram.hpp"
#include "Text.hpp"

namespace sge::Graphics {
  class FontRenderer {
    Font _font;
    Ref<ShaderProgram> _program;
    unsigned _vao, _vbo;
    std::vector<Text> _queue;

  public:
    FontRenderer(const Font& font, const Ref<ShaderProgram> program);

    void Add(Text text);
    void Render(const Text& text);
    void Draw();
    glm::vec2 CheckSize(const Text& text);
    const Font& GetFont() const { return _font; }
  };
}