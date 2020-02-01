#pragma once

#include "Button.hpp"

#include <vector>

namespace sge::Graphics {

  class ButtonClickManager {
    std::vector<const Button*> _buttons{};

  public:
    ButtonClickManager();

    void Add(const Button* button);
    void Clear();

    void HandleClick(float x, float y);
  };

}