#include "Font.hpp"

#include <glad/glad.h>

sge::Graphics::Font::Font(const std::string &path, int size, FT_Library ft) : _size(size) {
  FT_Face face;
  if (FT_New_Face(ft, "../fonts/arial.ttf", 0, &face)) printf("Failed to load arial.ttf\n");

  FT_Set_Pixel_Sizes(face, 0, size);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  for (GLubyte c = 0; c < 128; c++) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      printf("Failed to load Glyph %uc\n", c);
      continue;
    }

    unsigned texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Character character{
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        static_cast<unsigned int>(face->glyph->advance.x)
    };
    _characters.insert({c, character});
  }
  FT_Done_Face(face);
}
