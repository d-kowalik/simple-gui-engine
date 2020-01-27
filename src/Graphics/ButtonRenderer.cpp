#include "ButtonRenderer.hpp"

sge::Graphics::ButtonRenderer::ButtonRenderer(sge::Graphics::FontRenderer *font_renderer,
                                              sge::Graphics::RectangleRenderer *rectangle_renderer) : _font_renderer(
    font_renderer), _rectangle_renderer(rectangle_renderer) {

}

void sge::Graphics::ButtonRenderer::DrawButton(const Button& button) {
  const auto text = button.text;
  const auto position = button.position;
  const auto size = button.scale;
  const auto fill_color = button.fill_color;
  const auto text_color = button.text_color;
  const auto text_scale = button.text_scale;
  Text stext;
  stext.text = text;
  stext.scale = text_scale;
  stext.color = text_color;

  Rectangle rectangle;
  rectangle.scale = size;
  rectangle.position = position;
  rectangle.color = fill_color;

  auto height = _font_renderer->GetFont().GetSize();
  auto final_size = _font_renderer->CheckSize(stext);
  _rectangle_renderer->Draw(rectangle);

  glm::vec2 text_position{position.x + size.x / 2 - final_size.x / 2, position.y + size.y / 2 - final_size.y / 2};
  stext.position = text_position;

  _font_renderer->Render(stext);
}

