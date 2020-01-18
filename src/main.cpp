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

#include "Application.hpp"

using namespace sge;

class Program : public sge::Application {
  using sge::Application::Application;

  std::vector<std::vector<int>> _board{};
  int field_size = 32.0f;
  int margin = 6.0f;
  int fields_w = 0, fields_h = 0;

  bool OnCreate() override {
    int w = Window::Instance()->GetWidth();
    int h = Window::Instance()->GetHeight();
    fields_w = w/(field_size+margin);
    fields_h = h/(field_size+margin);
    _board.reserve(fields_w);
    for (auto& board : _board) {
      board.reserve(fields_h);
    }

    _board = std::vector<std::vector<int>>(fields_w, std::vector<int>(fields_h, 0));

    _board[3][3] = 1;

    return true;
  }

  bool OnUpdate(float delta) override {
    if (Input::IsKeyPressed(Key::ESCAPE)) Window::Instance()->Close();
    if (Input::IsKeyPressed(Key::A)) _camera->Move(Camera::Direction::LEFT, delta);
    if (Input::IsKeyPressed(Key::D)) _camera->Move(Camera::Direction::RIGHT, delta);
    if (Input::IsKeyPressed(Key::W)) _camera->Move(Camera::Direction::UP, delta);
    if (Input::IsKeyPressed(Key::S)) _camera->Move(Camera::Direction::DOWN, delta);


    for (int y = 0; y < fields_h; y++) {
      for (int x = 0; x < fields_w; x++) {
        if (_board[x][y] == 1) {
          DrawRectangle({field_size, field_size}, {(margin * (x+1) + field_size * x),
                                                   (margin * (y+1) + field_size * y)}, {.9f, .0f, .6f});
        } else {
          DrawRectangle({field_size, field_size}, {(margin * (x+1) + field_size * x),
                                                   (margin * (y+1) + field_size * y)}, {.0f, .0f, .0f});
        }
      }
    }

    return true;
  }
};

int main() {
  Program program{"Simple Gui Engine", 640, 480};
  program.Run();

  return 0;
}