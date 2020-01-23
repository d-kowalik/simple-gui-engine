#pragma once

#include "Graphics/FontRenderer.hpp"
#include "Graphics/RectangleRenderer.hpp"
#include "Graphics/ShaderProgram.hpp"
#include "Camera.hpp"
#include "Util/Callback.hpp"
#include "Graphics/ButtonRenderer.hpp"

namespace sge {
  class Application {
    inline static Application *_instance = nullptr;
  public:
    Graphics::FontRenderer *_font_renderer = nullptr;
    Graphics::RectangleRenderer *_rectangle_renderer = nullptr;
    Graphics::ShaderProgram *_font_program = nullptr, *_rectangle_program = nullptr;
    Graphics::ButtonRenderer *_button_renderer;
    Camera *_camera;
    glm::mat4 _view, _model, _projection;
    std::vector<std::pair<glm::vec4, std::function<void(float, float)>>> _click_bounds{};
  public:
    explicit Application(const std::string &title, int width, int height);

    virtual ~Application();

    virtual bool OnCreate() = 0;

    virtual bool OnUpdate(float delta) = 0;

    void Run();

    inline static Application *Instance() { return _instance; }

    void DrawRectangle(glm::vec2 scale, glm::vec2 pos, glm::vec3 color) const;

    void DrawRectangle(glm::vec2 scale, glm::vec2 pos, glm::vec3 color, std::function<void(float, float)> callback);

    void DrawButton(const std::string &text, glm::vec2 position, glm::vec2 size, glm::vec3 fill_color = {0.f, 0.f, 0.f},
                    glm::vec3 text_color = {1.f, 1.f, 1.f});
    void DrawButton(const std::string &text, glm::vec2 position, glm::vec2 size, glm::vec3 fill_color,
                    glm::vec3 text_color, std::function<void(float, float)> callback);

    void DrawText(const std::string &text, glm::vec2 position, float scale, glm::vec3 color) const;

    void HandleMouseClick(float x, float y);
    void HandleResize(int x, int y);
  };
}

