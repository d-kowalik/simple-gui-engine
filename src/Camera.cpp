#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

sge::Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float speed) : _position(position), _up(up),
                                                                                      _speed(speed), _front(front) {
  Update();
}

glm::mat4 sge::Camera::View() const {
  return glm::lookAt(_position, _position + _front, _up);
}

sge::Camera::Camera(float speed) noexcept : _speed(speed) {
  Update();
}

void sge::Camera::Move(sge::Camera::Direction direciton, float delta) {
  float velocity = _speed * delta;
  switch (direciton) {
    case Direction::LEFT:
      _position -= _right * velocity;
      break;
    case Direction::RIGHT:
      _position += _right * velocity;
      break;
    case Direction::UP:
      _position += _up * velocity;
      break;
    case Direction::DOWN:
      _position -= _up*velocity;
      break;
  }
}

void sge::Camera::Update() {
  _right = glm::cross(_front, _up);
}

