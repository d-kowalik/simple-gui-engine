#include <cstdio>
#include <glm/gtc/matrix_transform.hpp>

#include "Graphics/Window.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/ShaderProgram.hpp"
#include "Input/Input.hpp"

using namespace sge;

int main() {
  Window::Create(640, 480, "Simple GUI Engine");

  const auto vertex_shader = Graphics::Shader(GL_VERTEX_SHADER, "../src/Shaders/flat_color.vert");
  const auto fragment_shader = Graphics::Shader(GL_FRAGMENT_SHADER, "../src/Shaders/flat_color.frag");
  const auto shader_program = Graphics::ShaderProgram({vertex_shader, fragment_shader});

  float vertices[] = {
      .0f, .0f, 1.f, 1.f, .5f, 1.f,
      .0f, 50.f, 1.f, .5f, 1.f, 1.f,
      50.f, 50.f, 1.f, 1.f, .5f, .5f,
      50.f, .0f, 1.f, 1.f, 1.f, .5f
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glm::vec3 camera_position = glm::vec3(.0f, .0f, 3.0f);
  glm::vec3 camera_front = glm::vec3(.0f, .0f, -1.f);
  glm::vec3 camera_up = glm::vec3(.0f, 1.0f, .0f);

  glm::mat4 view = glm::lookAt(camera_position, camera_position + camera_front, camera_up);
  glm::mat4 projection = glm::ortho(0.0f, 640.f, 0.0f, 480.f, 0.1f, 10.0f);

  shader_program.Use();

  shader_program.SetUniformMat4f("view", view);
  shader_program.SetUniformMat4f("projection", projection);

  const float camera_speed = .5f;
  while (!Window::Instance()->ShouldClose()) {
    Window::Instance()->Clear();

    if (Input::IsKeyPressed(Key::ESCAPE)) Window::Instance()->Close();
    if (Input::IsKeyPressed(Key::A)) camera_position -= camera_speed * glm::cross(camera_front, camera_up);
    if (Input::IsKeyPressed(Key::D)) camera_position += camera_speed * glm::cross(camera_front, camera_up);
    if (Input::IsKeyPressed(Key::W)) camera_position += camera_speed * camera_up;
    if (Input::IsKeyPressed(Key::S)) camera_position -= camera_speed * camera_up;
    view = glm::lookAt(camera_position, camera_position + camera_front, camera_up);

    shader_program.Use();
    shader_program.SetUniformMat4f("view", view);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    Window::Instance()->Update();
  }

  Window::Destroy();

  return 0;
}