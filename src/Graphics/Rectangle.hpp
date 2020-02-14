#pragma once

#include "RectangleDefaults.hpp"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace sge::Graphics {
  struct Rectangle {
    glm::vec2 scale = RectangleDefaults::SCALE;
    glm::vec2 position = RectangleDefaults::POSITION;
    glm::vec3 color = RectangleDefaults::COLOR;

    glm::mat4 GetModel() const {
      glm::mat4 model(1.0f);
      glm::vec3 translate{ position.x, position.y, .0f };
      glm::vec3 vscale{ scale.x, scale.y, 1.0f };
      model = glm::translate(model, translate);
      model = glm::scale(model, vscale);
      return model;
    }
  };
}