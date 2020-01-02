#include <GLFW/glfw3.h>

#include <string>

namespace sge {
  class Window {
  private:
    GLFWwindow *_window;
    int _width, _height;
    std::string _title;

    void Resize(int w, int h);

  public:
    Window(int width, int height, std::string title);

    inline bool ShouldClose() const { return glfwWindowShouldClose(_window); }

    void Update() const;

    void Clear() const;

    void Close() const;

    friend void framebuffer_size_callback(GLFWwindow *window, int w, int h);
  };
} // namespace sge