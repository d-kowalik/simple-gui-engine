#include <cstdio>

#include <glm/gtc/matrix_transform.hpp>

#include "Graphics/Font.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/ShaderProgram.hpp"
#include "Input/Input.hpp"
#include "Camera.hpp"
#include "Graphics/FontRenderer.hpp"
#include "Graphics/RectangleRenderer.hpp"

using namespace sge;

Camera camera{10.f};

void HandleInput(float delta) {
  if (Input::IsKeyPressed(Key::ESCAPE)) Window::Instance()->Close();
  if (Input::IsKeyPressed(Key::A)) camera.Move(Camera::Direction::LEFT, delta);
  if (Input::IsKeyPressed(Key::D)) camera.Move(Camera::Direction::RIGHT, delta);
  if (Input::IsKeyPressed(Key::W)) camera.Move(Camera::Direction::UP, delta);
  if (Input::IsKeyPressed(Key::S)) camera.Move(Camera::Direction::DOWN, delta);
}

int main() {
  Window::Create(640, 480, "Simple GUI Engine");

  const auto vertex_shader = Graphics::Shader(GL_VERTEX_SHADER, "../src/Shaders/flat_color.vert");
  const auto fragment_shader = Graphics::Shader(GL_FRAGMENT_SHADER, "../src/Shaders/flat_color.frag");
  const auto shader_program = Graphics::ShaderProgram({vertex_shader, fragment_shader});

  const auto text_vertex_shader = Graphics::Shader(GL_VERTEX_SHADER, "../src/Shaders/text.vert");
  const auto text_fragment_shader = Graphics::Shader(GL_FRAGMENT_SHADER, "../src/Shaders/text.frag");
  const auto text_shader_program = Graphics::ShaderProgram({text_vertex_shader, text_fragment_shader});

  FT_Library ft;
  if (FT_Init_FreeType(&ft)) printf("Could not init FreeType\n");
  Graphics::Font arial{"../fonts/arial.ttf", 48, ft};
  Graphics::FontRenderer font_renderer{arial, text_shader_program};
  FT_Done_FreeType(ft);

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

  Graphics::RectangleRenderer rectangle_renderer{shader_program};

  float t = 0.f;
  float dt = 1.0f / 60.f;
  while (!Window::Instance()->ShouldClose()) {
    Window::Instance()->Clear();

    HandleInput(dt);

    view = camera.View();
    model = glm::mat4(1.0f);

    shader_program.Use();
    shader_program.SetUniformMat4f("view", view);
    shader_program.SetUniformMat4f("model", model);
    text_shader_program.Use();
    text_shader_program.SetUniformMat4f("view", view);
    font_renderer.Render("ddddTest tekstu yzxvbniJQ", {.0f, .0f}, 1.0f,
                         glm::vec3((std::sin(glfwGetTime()) + 1) / 2, (std::cos(glfwGetTime() + 1) / 2),
                                   std::sin(glfwGetTime())));

    shader_program.Use();
    // Button ?
    rectangle_renderer.Draw({300.f, 200.f}, {50.f, 50.f}, {.3f, .7f, .9f});
    font_renderer.Render("Przycisk", {50.f + (300.f/2.f) - (48.f/2), 50.f + (200.f/2.f) - (48.f / 2.f)}, 1.0f, {0.f, 0.f, 0.f});

    t += dt;
    Window::Instance()->Update();
  }

  Window::Destroy();

  return 0;
}