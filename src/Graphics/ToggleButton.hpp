#pragma once

#include "Button.hpp"
#include "ToggleButtonDefaults.hpp"

namespace sge::Graphics {

  struct ToggleButton : public Button {
    bool toggled = ToggleButtonDefaults::TOGGLED;
    glm::vec3 toggle_color =ToggleButtonDefaults::TOGGLE_COLOR;
  };

}