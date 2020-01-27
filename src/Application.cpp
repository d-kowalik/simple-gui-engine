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

  const auto vertex_shader = Graphics::Shader::CreateFromRawData(GL_VERTEX_SHADER, "#version 330 core\n"
                                                                                   "\n"
                                                                                   "layout (location = 0) in vec3 position;\n"
                                                                                   "layout (location = 1) in vec3 color;\n"
                                                                                   "\n"
                                                                                   "uniform mat4 model;\n"
                                                                                   "uniform mat4 view;\n"
                                                                                   "uniform mat4 projection;\n"
                                                                                   "\n"
                                                                                   "out vec3 Color;\n"
                                                                                   "\n"
                                                                                   "void main() {\n"
                                                                                   "    gl_Position = projection * view * model * vec4(position, 1.0);\n"
                                                                                   "    Color = color;\n"
                                                                                   "}");
  const auto fragment_shader = Graphics::Shader::CreateFromRawData(GL_FRAGMENT_SHADER, "#version 330 core\n"
                                                                    "\n"
                                                                    "in vec3 Color;\n"
                                                                    "\n"
                                                                    "out vec4 FinalColor;\n"
                                                                    "\n"
                                                                    "void main() {\n"
                                                                    "    FinalColor = vec4(Color, 1.0);\n"
                                                                    "}");
  _rectangle_program =new  Graphics::ShaderProgram({vertex_shader, fragment_shader});

  const auto text_vertex_shader = Graphics::Shader::CreateFromRawData(GL_VERTEX_SHADER, "#version 330 core\n"
                                                                     "\n"
                                                                     "layout (location = 0) in vec4 vertex;\n"
                                                                     "out vec2 TexCoords;\n"
                                                                     "\n"
                                                                     "uniform mat4 projection;\n"
                                                                     "uniform mat4 view;\n"
                                                                     "\n"
                                                                     "void main() {\n"
                                                                     "    gl_Position = projection * view * vec4(vertex.xy, 1.f, 1.f);\n"
                                                                     "    TexCoords = vertex.zw;\n"
                                                                     "}");
  const auto text_fragment_shader = Graphics::Shader::CreateFromRawData(GL_FRAGMENT_SHADER, "#version 330 core\n"
                                                                         "\n"
                                                                         "in vec2 TexCoords;\n"
                                                                         "out vec4 color;\n"
                                                                         "\n"
                                                                         "uniform sampler2D text;\n"
                                                                         "uniform vec3 textColor;\n"
                                                                         "\n"
                                                                         "void main() {\n"
                                                                         "    vec4 sampled = vec4(1.f, 1.f, 1.f, texture(text, TexCoords).r);\n"
                                                                         "    color = vec4(textColor, 1.f) * sampled;\n"
                                                                         "}");
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
  Window::Instance()->OnResize += std::bind(&Application::HandleResize, this, std::placeholders::_1, std::placeholders::_2);
}

void sge::Application::Run() {
  OnCreate();

  while (!Window::Instance()->ShouldClose()) {
    _timer.Tick();
    Window::Instance()->UpdateTitle(_timer.GetFPS());
    Window::Instance()->Clear();
    OnUpdate(_timer.GetDelta());

    _view = _camera->View();
    _rectangle_program->Use();
    _rectangle_program->SetUniformMat4f("view", _view);
    _font_program->Use();
    _font_program->SetUniformMat4f("view", _view);


    Window::Instance()->Update();
    _click_bounds.clear();
  }
}

void sge::Application::DrawRectangle(const Graphics::Rectangle& rectangle,
                                     std::function<void(float, float)> callback) {
  const auto pos = rectangle.position;
  const auto scale = rectangle.scale;
  _click_bounds.emplace_back(glm::vec4{pos.x, pos.y, scale.x + pos.x, scale.y + pos.y}, callback);
  DrawRectangle(rectangle);
}

void sge::Application::DrawRectangle(const Graphics::Rectangle& rectangle) const {
  _rectangle_program->Use();
  _rectangle_renderer->Draw(rectangle);
}

void sge::Application::DrawText(const Graphics::Text &text) const {
  _font_program->Use();
  _font_renderer->Render(text);
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
                                  glm::vec3 text_color, float text_scale) {
  _rectangle_program->Use();
  _button_renderer->DrawButton(text, position, size, fill_color, text_color, text_scale);
}

void sge::Application::DrawButton(const std::string &text, glm::vec2 position, glm::vec2 size, glm::vec3 fill_color,
                                  glm::vec3 text_color, float text_scale, std::function<void(float, float)> callback) {
  _click_bounds.emplace_back(glm::vec4{position.x, position.y, size.x + position.x, size.y + position.y}, callback);
  _rectangle_program->Use();
  _button_renderer->DrawButton(text, position, size, fill_color, text_color, text_scale);
}

void sge::Application::HandleResize(int w, int h) {
  _projection = glm::ortho(0.0f, (float)w, 0.0f, (float)h, 0.1f, 10.0f);
  _rectangle_program->Use();
  _rectangle_program->SetUniformMat4f("projection", _projection);
  _font_program->Use();
  _font_program->SetUniformMat4f("projection", _projection);
}

