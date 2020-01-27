#pragma once

#include "FontRenderer.hpp"
#include "RectangleRenderer.hpp"

namespace sge::Graphics {
  class ButtonRenderer {
    constexpr static glm::vec3 DEFAULT_FILL_COLOR = {0.f, 0.f, 0.f};
    constexpr static glm::vec3 DEFAULT_TEXT_COLOR = {1.f, 1.f, 1.f};

    FontRenderer *_font_renderer;
    RectangleRenderer *_rectangle_renderer;

  public:
    ButtonRenderer(FontRenderer *font_renderer, RectangleRenderer *rectangle_renderer);

    void DrawButton(const std::string &text, glm::vec2 position, glm::vec2 size, glm::vec3 fill_color = DEFAULT_FILL_COLOR,
                    glm::vec3 text_color = DEFAULT_TEXT_COLOR, float text_scale = 1.0f);
  };

}

