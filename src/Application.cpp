//
// Created by damian on 16.01.2020.
//

#include "Application.hpp"

#include "Graphics/Window.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

sge::Application::Application(const std::string &title, int width, int height) {
  Window::Create(width, height, title);

  _camera = new Camera{100.f};

  const auto vertex_shader = Graphics::Shader(GL_VERTEX_SHADER, "../src/Shaders/flat_color.vert");
  const auto fragment_shader = Graphics::Shader(GL_FRAGMENT_SHADER, "../src/Shaders/flat_color.frag");
  _rectangle_program =new  Graphics::ShaderProgram({vertex_shader, fragment_shader});

  const auto text_vertex_shader = Graphics::Shader(GL_VERTEX_SHADER, "../src/Shaders/text.vert");
  const auto text_fragment_shader = Graphics::Shader(GL_FRAGMENT_SHADER, "../src/Shaders/text.frag");
  _font_program = new Graphics::ShaderProgram({text_vertex_shader, text_fragment_shader});

  FT_Library ft;
  if (FT_Init_FreeType(&ft)) printf("Could not init FreeType\n");
  Graphics::Font arial{"../fonts/arial.ttf", 48, ft};
  _font_renderer = new Graphics::FontRenderer {arial, *_font_program};
  FT_Done_FreeType(ft);

  _view = _camera->View();
  _projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 10.0f);
  _model = glm::mat4(1.0f);

  _rectangle_program->Use();
  _rectangle_program->SetUniformMat4f("model", _model);
  _rectangle_program->SetUniformMat4f("view", _view);
  _rectangle_program->SetUniformMat4f("projection", _projection);

  _font_program->Use();
  _font_program->SetUniformMat4f("projection", _projection);
  _font_program->SetUniformMat4f("view", _view);

  _rectangle_renderer = new Graphics::RectangleRenderer {*_rectangle_program};

  _button_renderer = new Graphics::ButtonRenderer(_font_renderer, _rectangle_renderer);

  _instance = this;

  Window::Instance()->OnMouseClick += std::bind(&Application::HandleMouseClick, this, std::placeholders::_1, std::placeholders::_2);
}

void sge::Application::Run() {
  OnCreate();

  float t = 0.f;
  float dt = 1.0f / 60.f;
  while (!Window::Instance()->ShouldClose()) {
    Window::Instance()->Clear();
    OnUpdate(dt);

    _view = _camera->View();
    _rectangle_program->Use();
    _rectangle_program->SetUniformMat4f("view", _view);
    _font_program->Use();
    _font_program->SetUniformMat4f("view", _view);


    t += dt;
    Window::Instance()->Update();
    _click_bounds.clear();
  }
}

void sge::Application::DrawRectangle(glm::vec2 scale, glm::vec2 pos, glm::vec3 color,
                                     std::function<void(float, float)> callback) {
  _click_bounds.emplace_back(glm::vec4{pos.x, pos.y, scale.x + pos.x, scale.y + pos.y}, callback);
  DrawRectangle(scale, pos, color);
}

void sge::Application::DrawRectangle(glm::vec2 scale, glm::vec2 pos, glm::vec3 color) const {
  _rectangle_program->Use();
  _rectangle_renderer->Draw(scale, pos, color);
}

void sge::Application::DrawText(const std::string &text, glm::vec2 position, float scale, glm::vec3 color) const {
  _font_program->Use();
  _font_renderer->Render(text, position, scale, color);
}

sge::Application::~Application() {
  delete _font_renderer;
  delete _rectangle_renderer;
  delete _font_program;
  delete _rectangle_program;
  delete _camera;
  Window::Destroy();
}

void sge::Application::HandleMouseClick(float x, float y) {
  for (const auto& [points, func] : _click_bounds) {
    if (x >= points.x && x <= points.z && y >= points.y && y <= points.w) {
      func(x, y);
    }
  }
}

void sge::Application::DrawButton(const std::string &text, glm::vec2 position, glm::vec2 size, glm::vec3 fill_color,
                                  glm::vec3 text_color) {
  _rectangle_program->Use();
  _button_renderer->DrawButton(text, position, size, fill_color, text_color);
}

void sge::Application::DrawButton(const std::string &text, glm::vec2 position, glm::vec2 size, glm::vec3 fill_color,
                                  glm::vec3 text_color, std::function<void(float, float)> callback) {
  _click_bounds.emplace_back(glm::vec4{position.x, position.y, size.x + position.x, size.y + position.y}, callback);
  _rectangle_program->Use();
  _button_renderer->DrawButton(text, position, size, fill_color, text_color);
}


