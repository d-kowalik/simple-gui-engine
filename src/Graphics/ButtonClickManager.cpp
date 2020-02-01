//
// Created by damian on 01.02.2020.
//

#include "ButtonClickManager.hpp"

#include "ToggleButton.hpp"

#include "Window.hpp"

#include <functional>

sge::Graphics::ButtonClickManager::ButtonClickManager() {
  Window::Instance()->OnMouseClick += std::bind(&ButtonClickManager::HandleClick, this, std::placeholders::_1,
                                                std::placeholders::_2);
}

void sge::Graphics::ButtonClickManager::HandleClick(float x, float y) {
  bool toggle_check = false;
  for (Graphics::Button* button : _buttons) {
    if (x >= button->position.x && y >= button->position.y && x <= button->position.x + button->scale.x &&
        y <= button->position.y + button->scale.y) {
      if (dynamic_cast<Graphics::ToggleButton *>(button) != nullptr)  {
        toggle_check = true;
        dynamic_cast<Graphics::ToggleButton *>(button)->toggled = true;
      }
      button->callback(x, y);
    }
  }

  if (toggle_check) {
    for (Graphics::Button* button : _buttons) {
      if (!(x >= button->position.x && y >= button->position.y && x <= button->position.x + button->scale.x &&
          y <= button->position.y + button->scale.y)) {
        if (dynamic_cast<Graphics::ToggleButton *>(button) != nullptr)  {
          dynamic_cast<Graphics::ToggleButton *>(button)->toggled = false;
        }
      }
    }
  }
}

void sge::Graphics::ButtonClickManager::Add(sge::Graphics::Button *button) {
  _buttons.push_back(button);
}

void sge::Graphics::ButtonClickManager::Clear() {
  _buttons.clear();
}
