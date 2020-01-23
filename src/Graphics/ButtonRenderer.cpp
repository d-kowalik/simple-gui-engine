#include "ButtonRenderer.hpp"

sge::Graphics::ButtonRenderer::ButtonRenderer(sge::Graphics::FontRenderer *font_renderer,
                                              sge::Graphics::RectangleRenderer *rectangle_renderer) : _font_renderer(
    font_renderer), _rectangle_renderer(rectangle_renderer) {

}

void sge::Graphics::ButtonRenderer::DrawButton(const std::string &text, glm::vec2 position, glm::vec2 size,
                                               glm::vec3 fill_color, glm::vec3 text_color) {
  auto height = _font_renderer->GetFont().GetSize();
  auto final_size = _font_renderer->CheckSize(text, 1);
  _rectangle_renderer->Draw(size, position, fill_color);
  _font_renderer->Render(text,
                         {position.x + size.x / 2 - final_size.x / 2, position.y + size.y / 2 - final_size.y / 2}, 1.f,
                         text_color);
}
