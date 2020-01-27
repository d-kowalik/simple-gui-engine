#pragma once

#include "FontRenderer.hpp"
#include "RectangleRenderer.hpp"
#include "Button.hpp"

namespace sge::Graphics {
  class ButtonRenderer {
    FontRenderer *_font_renderer;
    RectangleRenderer *_rectangle_renderer;

  public:
    ButtonRenderer(FontRenderer *font_renderer, RectangleRenderer *rectangle_renderer);

    void DrawButton(const Button& button);
  };

}

