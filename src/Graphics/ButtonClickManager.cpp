//
// Created by damian on 01.02.2020.
//

#include "ButtonClickManager.hpp"

#include "Window.hpp"

#include <functional>

sge::Graphics::ButtonClickManager::ButtonClickManager() {
  Window::Instance()->OnMouseClick += std::bind(&ButtonClickManager::HandleClick, this, std::placeholders::_1,
                                                std::placeholders::_2);
}

void sge::Graphics::ButtonClickManager::HandleClick(float x, float y) {
  bool toggle_switch = false;
  Button *toggled_button = nullptr;
  for (auto *button : _buttons) {
    if (x >= button->position.x && y >= button->position.y && x <= button->position.x + button->scale.x &&
        y <= button->position.y + button->scale.y) {
      if (button->is_toggle_button) {
        toggle_switch = true;
        button->toggled = true;
        toggled_button = button;
      }
      button->callback(x, y);
    }
  }

  if (toggle_switch)
    for (auto *button : _buttons)
      if (button != toggled_button)
        button->toggled = false;
}

void sge::Graphics::ButtonClickManager::Add(sge::Graphics::Button &button) {
  _buttons.push_back(&button);
}

void sge::Graphics::ButtonClickManager::Clear() {
  _buttons.clear();
}
