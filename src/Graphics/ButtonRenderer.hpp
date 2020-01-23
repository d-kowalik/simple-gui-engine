#pragma once

#include "FontRenderer.hpp"
#include "RectangleRenderer.hpp"

namespace sge::Graphics {
  class ButtonRenderer {
    FontRenderer *_font_renderer;
    RectangleRenderer *_rectangle_renderer;

  public:
    ButtonRenderer(FontRenderer *font_renderer, RectangleRenderer *rectangle_renderer);

    void DrawButton(const std::string &text, glm::vec2 position, glm::vec2 size, glm::vec3 fill_color = {0.f, 0.f, 0.f},
                    glm::vec3 text_color = {1.f, 1.f, 1.f});
  };

}

