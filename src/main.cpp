#include <cstdio>
#include <map>

#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Graphics/Window.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/ShaderProgram.hpp"
#include "Input/Input.hpp"
#include "Camera.hpp"

using namespace sge;

Camera camera{0.5f};

void HandleInput(float delta) {
  if (Input::IsKeyPressed(Key::ESCAPE)) Window::Instance()->Close();
  if (Input::IsKeyPressed(Key::A)) camera.Move(Camera::Direction::LEFT, delta);
  if (Input::IsKeyPressed(Key::D)) camera.Move(Camera::Direction::RIGHT, delta);
  if (Input::IsKeyPressed(Key::W)) camera.Move(Camera::Direction::UP, delta);
  if (Input::IsKeyPressed(Key::S)) camera.Move(Camera::Direction::DOWN, delta);
}

struct Character {
  unsigned texture_id;
  glm::ivec2 size;
  glm::ivec2 bearing;
  unsigned advance;
};

std::map<char, Character> characters;
unsigned text_vao, text_vbo;

void
RenderText(const Graphics::ShaderProgram &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
  // Activate corresponding render state
  s.Use();
  s.SetUniform3f("textColor", color);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(text_vao);

  // Iterate through all characters
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) {
    Character ch = characters[*c];

    GLfloat xpos = x + ch.bearing.x * scale;
    GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

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
    glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
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

int main() {
  Window::Create(640, 480, "Simple GUI Engine");

  const auto vertex_shader = Graphics::Shader(GL_VERTEX_SHADER, "../src/Shaders/flat_color.vert");
  const auto fragment_shader = Graphics::Shader(GL_FRAGMENT_SHADER, "../src/Shaders/flat_color.frag");
  const auto shader_program = Graphics::ShaderProgram({vertex_shader, fragment_shader});

  const auto text_vertex_shader = Graphics::Shader(GL_VERTEX_SHADER, "../src/Shaders/text.vert");
  const auto text_fragment_shader = Graphics::Shader(GL_FRAGMENT_SHADER, "../src/Shaders/text.frag");
  const auto text_shader_program = Graphics::ShaderProgram({text_vertex_shader, text_fragment_shader});

  float vertices[] = {
      .0f, .0f, .5f, 1.f, .5f, 1.f,
      .0f, 50.f, .5f, .5f, 1.f, 1.f,
      50.f, 50.f, .5f, 1.f, .5f, .5f,
      50.f, .0f, .5f, 1.f, 1.f, .5f
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  FT_Library ft;
  if (FT_Init_FreeType(&ft)) printf("Could not init FreeType\n");

  FT_Face face;
  if (FT_New_Face(ft, "../fonts/arial.ttf", 0, &face)) printf("Failed to load arial.ttf\n");

  FT_Set_Pixel_Sizes(face, 0, 48);
  if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) printf("Failed to load Glyph\n");

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
    // Now store character for later use
    Character character{
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        static_cast<unsigned int>(face->glyph->advance.x)
    };
    characters.insert({c, character});
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glGenVertexArrays(1, &text_vao);
  glGenBuffers(1, &text_vbo);
  glBindVertexArray(text_vao);
  glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glm::mat4 view = camera.View();
  glm::mat4 projection = glm::ortho(0.0f, 640.f, 0.0f, 480.f, 0.1f, 10.0f);
  glm::mat4 model(1.0f);

  shader_program.Use();
  shader_program.SetUniformMat4f("model", model);
  shader_program.SetUniformMat4f("view", view);
  shader_program.SetUniformMat4f("projection", projection);

  text_shader_program.Use();
  text_shader_program.SetUniformMat4f("projection", projection);
  text_shader_program.SetUniformMat4f("view", view);

  while (!Window::Instance()->ShouldClose()) {
    Window::Instance()->Clear();

    HandleInput(1.0f);

    view = camera.View();
    model = glm::rotate(model, glm::radians(0.05f), {.0f, .0f, 1.f});

    shader_program.Use();
    shader_program.SetUniformMat4f("view", view);
    shader_program.SetUniformMat4f("model", model);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    text_shader_program.Use();
    text_shader_program.SetUniformMat4f("view", view);
    RenderText(text_shader_program, "Test tekstu gxyzzurwqdf", .0f, 0.0f, 1.0f,
               glm::vec3((std::sin(glfwGetTime()) + 1) / 2, (std::cos(glfwGetTime() + 1) / 2),
                         std::sin(glfwGetTime())));

    Window::Instance()->Update();
  }

  Window::Destroy();

  return 0;
}