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

  bool OnCreate() override {

  }

  bool OnUpdate(float delta) override {
    if (Input::IsKeyPressed(Key::ESCAPE)) Window::Instance()->Close();
    if (Input::IsKeyPressed(Key::A)) _camera->Move(Camera::Direction::LEFT, delta);
    if (Input::IsKeyPressed(Key::D)) _camera->Move(Camera::Direction::RIGHT, delta);
    if (Input::IsKeyPressed(Key::W)) _camera->Move(Camera::Direction::UP, delta);
    if (Input::IsKeyPressed(Key::S)) _camera->Move(Camera::Direction::DOWN, delta);

    DrawText("ddddTest tekstu yzxvbniJQ", {.0f, .0f}, 1.0f,
                           glm::vec3((std::sin(glfwGetTime()) + 1) / 2, (std::cos(glfwGetTime() + 1) / 2),
                                     std::sin(glfwGetTime())));

    DrawRectangle({300.f, 200.f}, {50.f, 50.f}, {.3f, .7f, .9f});
    DrawText("Przycisk", {50.f + (300.f/2.f) - 48.f, 50.f + (200.f/2.f) - (48.f / 2.f)}, 1.0f, {0.f, 0.f, 0.f});
  }
};

int main() {
  Program program{"Simple Gui Engine", 640, 480};
  program.Run();

  return 0;
}