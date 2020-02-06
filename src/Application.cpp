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
  _button_click_manager = new Graphics::ButtonClickManager();

  _instance = this;

  Window::Instance()->OnResize += std::bind(&Application::HandleResize, this, std::placeholders::_1, std::placeholders::_2);
  Window::Instance()->OnButtonPressed += std::bind(&Application::HandleKeyPressed, this, std::placeholders::_1);
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
    _button_click_manager->Clear();
  }
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

void sge::Application::HandleResize(int w, int h) {
  _projection = glm::ortho(0.0f, (float)w, 0.0f, (float)h, 0.1f, 10.0f);
  _rectangle_program->Use();
  _rectangle_program->SetUniformMat4f("projection", _projection);
  _font_program->Use();
  _font_program->SetUniformMat4f("projection", _projection);
}

void sge::Application::DrawButton(sge::Graphics::Button& button) {
  _rectangle_program->Use();
  _button_renderer->DrawButton(button);
  _button_click_manager->Add(button);
}

