#include <glad/glad.h>

#include "FontRenderer.hpp"

sge::Graphics::FontRenderer::FontRenderer(const sge::Graphics::Font &font,
                                          const Ref<sge::Graphics::ShaderProgram> program) : _font(font), _program(program) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glGenVertexArrays(1, &_vao);
  glGenBuffers(1, &_vbo);
  glBindVertexArray(_vao);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void sge::Graphics::FontRenderer::Render(const sge::Graphics::Text &stext) {
  const auto color = stext.color;
  const auto text = stext.text;
  const auto position = stext.position;
  const auto scale = stext.scale;
  _program->Use();
  _program->SetUniform3f("textColor", color);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(_vao);

  const auto& characters = _font.GetCharacters();

  float x = position.x;
  float y = position.y;

  for (const auto& c : text) {
    Graphics::Font::Character ch = characters.at(c);

    float xpos = x + ch.bearing.x * scale;
    float ypos = y - (ch.size.y - ch.bearing.y) * scale;

    GLfloat w = ch.size.x * scale;
    GLfloat h = ch.size.y * scale;
    // Update VBO for each character
    GLfloat vertices[6][4] = {
        {xpos,     ypos + h, 0.0, 0.0},
        {xpos,     ypos,     0.0, 1.0},
        {xpos + w, ypos,     1.0, 1.0},

        {xpos,     ypos + h, 0.0, 0.0},
        {xpos + w, ypos,     1.0, 1.0},
        {xpos + w, ypos + h, 1.0, 0.0}
    };
    // Render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.texture_id);
    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

glm::vec2 sge::Graphics::FontRenderer::CheckSize(const Text& stext) {
  const auto text = stext.text;
  const auto scale = stext.scale;
  if (text.length() == 0) return {0, 0};
    float width = 0, height = 0;
    const auto& characters = _font.GetCharacters();
    height = characters.at(text[0]).size.y * scale;
    for (const auto& c : text) {
      Graphics::Font::Character ch = characters.at(c);
      width += (ch.advance >> 6) * scale;
    }
    return {width, height};
}
