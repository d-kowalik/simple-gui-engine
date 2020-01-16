#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <string>

namespace sge {
  class Window {
  private:
    static Window* _instance;

    GLFWwindow *_window;
    int _width, _height;
    std::string _title;

    void Resize(int w, int h);

    bool InitializeGLFW();

    bool CreateWindow();

    bool InitializeOpenGL();

    Window(int width, int height, std::string title);

  public:
    inline bool ShouldClose() const { return glfwWindowShouldClose(_window); }

    void Update() const;

    void Clear() const;

    void Close() const;

    inline int GetWidth() const { return _width; }
    inline int GetHeight() const { return _height; }

    GLFWwindow* GetNativeHandle() const { return _window; }

    static Window* Instance() { return _instance; }

    static Window* Create(int width, int height, const std::string& title);
    static void Destroy();

    friend void framebuffer_size_callback(GLFWwindow *window, int w, int h);
  };
} // namespace sge