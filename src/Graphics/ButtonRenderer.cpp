#include "ButtonRenderer.hpp"

sge::Graphics::ButtonRenderer::ButtonRenderer(sge::Graphics::FontRenderer *font_renderer,
                                              sge::Graphics::RectangleRenderer *rectangle_renderer) : _font_renderer(
    font_renderer), _rectangle_renderer(rectangle_renderer) {

}

void sge::Graphics::ButtonRenderer::DrawButton(const std::string &text, glm::vec2 position, glm::vec2 size,
                                               glm::vec3 fill_color, glm::vec3 text_color) {
  auto height = _font_renderer->GetFont().GetSize();
  const float text_scale = std::min(size.x / (float)(height * text.size()), size.y / height);
  auto final_size = _font_renderer->CheckSize(text, text_scale);
  _rectangle_renderer->Draw(size, position, fill_color);

  glm::vec2 text_position{position.x + size.x / 2 - final_size.x / 2, position.y + size.y / 2 - final_size.y / 2};
  _font_renderer->Render(text, text_position, text_scale, text_color);
}

