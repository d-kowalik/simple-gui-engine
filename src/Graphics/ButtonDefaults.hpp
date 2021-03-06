#pragma once

#include <string>
#include <glm/glm.hpp>

namespace sge::Graphics::ButtonDefaults {
  static const glm::vec2 POSITION = {.0f, .0f};
  static const glm::vec2 SCALE = {200.0f, 150.0f};
  static const float TEXT_SCALE = 1.f;
  static const glm::vec3 TEXT_COLOR = {1.0f, 1.f, 1.0f};
  static const glm::vec3 FILL_COLOR = {.7f, .3f, .3f};
  static const glm::vec3 PRESSED_COLOR = {.5f, .2f, .15f};
  static const glm::vec3 TOGGLED_COLOR = {.3f, .3f, .3f};
  constexpr bool TOGGLED = false;
  constexpr bool IS_TOGGLE_BUTTON = false;
}