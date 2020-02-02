#pragma once

#include "Graphics/FontRenderer.hpp"
#include "Graphics/RectangleRenderer.hpp"
#include "Graphics/ShaderProgram.hpp"
#include "Camera.hpp"
#include "Util/Callback.hpp"
#include "Graphics/ButtonRenderer.hpp"
#include "Util/Timer.hpp"
#include "Graphics/ButtonClickManager.hpp"

namespace sge {
  class Application {
    inline static Application *_instance = nullptr;
  public:
    Graphics::FontRenderer *_font_renderer = nullptr;
    Graphics::RectangleRenderer *_rectangle_renderer = nullptr;
    Graphics::ShaderProgram *_font_program = nullptr, *_rectangle_program = nullptr;
    Graphics::ButtonRenderer *_button_renderer;
    Graphics::ButtonClickManager* _button_click_manager = nullptr;
    Camera *_camera;
    glm::mat4 _view, _model, _projection;
    Util::Timer _timer{};
  public:
    explicit Application(const std::string &title, int width, int height);

    virtual ~Application();

    virtual bool OnCreate() = 0;

    virtual bool OnUpdate(float delta) = 0;

    void Run();

    inline static Application *Instance() { return _instance; }

    void DrawRectangle(const Graphics::Rectangle& rectangle = Graphics::Rectangle{}) const;

    void DrawText(const Graphics::Text& text) const;

    void DrawButton(Graphics::Button& button);

    void HandleResize(int x, int y);
  };
}

