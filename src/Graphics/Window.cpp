#include "Window.hpp"

#include <utility>

namespace sge {
  void framebuffer_size_callback(GLFWwindow *window, int w, int h);
  void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

  Window *Window::_instance = nullptr;

  Window::Window(int width, int height, std::string title) : _width(width), _height(height), _title(std::move(title)) {
    if (!InitializeGLFW()) {
      printf("Failed to initialize GLFW\n");
      return;
    }
    if (!CreateWindow()) {
      printf("Failed to create window");
      return;
    }
    if (!InitializeOpenGL()) {
      printf("Failed to init OpenGL");
    }
  }

  void Window::Update() const {
    glfwSwapBuffers(_window);
    glfwPollEvents();
  }

  void framebuffer_size_callback(GLFWwindow *window, int w, int h) {
    auto *win = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
    win->Resize(w, h);
  }

  void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    auto *win = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
    if ((action == GLFW_PRESS || action == GLFW_REPEAT) && button == GLFW_MOUSE_BUTTON_1) {
      double x, y;
      glfwGetCursorPos(window, &x, &y);
      y = std::abs(win->GetHeight() - y);
      win->OnMouseClick(x, y);
    }
  }


  void Window::Resize(int w, int h) {
    _width = w;
    _height = h;
    glViewport(0, 0, w, h);
    OnResize(w, h);
  }

  void Window::Clear() const {
    glClearColor(0.3f, 0.3f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void Window::Close() const {
    glfwSetWindowShouldClose(_window, true);
  }

  bool Window::InitializeGLFW() {
    if (!glfwInit()) return false;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSwapInterval(1);
    return true;
  }

  bool Window::CreateWindow() {
    _window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);
    if (!_window) {
      printf("Failed to create GLFW window\n");
      return false;
    }
    glfwMakeContextCurrent(_window);
    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(_window, mouse_button_callback);
    return true;
  }

  bool Window::InitializeOpenGL() {
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
      return false;
    }


    glViewport(0, 0, _width, _height);
    glEnable(GL_DEPTH_TEST);
    return true;
  }

  Window *Window::Create(int width, int height, const std::string &title) {
    Window::_instance = new Window(width, height, title);
    return Window::_instance;
  }

  void Window::Destroy() {
    if (_instance != nullptr)
      delete Window::_instance;
  }

  void Window::UpdateTitle(unsigned fps) {
    if (_last_fps == fps) return;
    _last_fps = fps;
    std::string newTitle{ _title + " | " + std::to_string(fps) + "fps" };
    glfwSetWindowTitle(_window, newTitle.c_str());
  }
} // namespace sge