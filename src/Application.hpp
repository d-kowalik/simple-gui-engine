#pragma once

#include "Graphics/FontRenderer.hpp"
#include "Graphics/RectangleRenderer.hpp"
#include "Graphics/ShaderProgram.hpp"
#include "Camera.hpp"
#include "Util/Callback.hpp"
#include "Graphics/ButtonRenderer.hpp"
#include "Util/Timer.hpp"

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
    Util::Timer _timer{};
  public:
    explicit Application(const std::string &title, int width, int height);

    virtual ~Application();

    virtual bool OnCreate() = 0;

    virtual bool OnUpdate(float delta) = 0;

    void Run();

    inline static Application *Instance() { return _instance; }

    void DrawRectangle(const Graphics::Rectangle& rectangle = Graphics::Rectangle{}) const;

    void DrawRectangle(const Graphics::Rectangle& rectangle, std::function<void(float, float)> callback);

    void DrawButton(const Graphics::Button& button);
    void DrawButton(const Graphics::Button& button, std::function<void(float, float)> callback);

    void DrawText(const Graphics::Text& text) const;

    void HandleMouseClick(float x, float y);
    void HandleResize(int x, int y);
  };
}

