#include "Input.hpp"

#include "../Graphics/Window.hpp"

bool sge::Input::IsKeyPressed(Key key) {
  const auto window_handle = Window::Instance()->GetNativeHandle();
  const auto state = glfwGetKey(window_handle, (int)key);

  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool sge::Input::IsMouseButtonPressed(sge::MouseButton button) {
  const auto window_handle = Window::Instance()->GetNativeHandle();
  const auto state = glfwGetMouseButton(window_handle, (int)button);

  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

glm::vec2 sge::Input::GetMousePos() {
  auto window_handle = Window::Instance()->GetNativeHandle();
  double x, y;
  glfwGetCursorPos(window_handle, &x, &y);
  return {x, y};
}
