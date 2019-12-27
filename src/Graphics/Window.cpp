#include "Window.hpp"

#include <utility>

namespace sge
{
void framebuffer_size_callback(GLFWwindow* window, int w, int h);

Window::Window(int width, int height, std::string title) : _width(width), _height(height), _title(std::move(title)) {
  if (!glfwInit())
    printf("Failed to initialize glfw!\n");

  _window = glfwCreateWindow(width, height, _title.c_str(), NULL, NULL);
  if (!_window) {
    glfwTerminate();
    printf("Failed to create window!");
  }

  glfwMakeContextCurrent(_window);

  glfwSetWindowUserPointer(_window, this);
  glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
}

void Window::Update() const {
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(_window);
  glfwPollEvents();
}
    void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
        auto *win = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
        win->Resize(w, h);
    }

    void Window::Resize(int w, int h) {
        _width = w;
        _height = h;
        glViewport(0, 0, w, h);
    }
} // namespace sge