#pragma once

#include "TextDefaults.hpp"

namespace sge::Graphics {
  struct Text {
    std::string text{};
    glm::vec2 position = TextDefaults::POSITION;
    glm::vec3 color = TextDefaults::COLOR;
    float scale = TextDefaults::SCALE;
  };
}