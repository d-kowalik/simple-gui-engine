#include <cstdio>

#include "Graphics/Window.hpp"
#include "Util/FileUtil.hpp"

using namespace sge;

int main() {
  Window window{640, 480, "Simple GUI Engine"};
  while (!window.ShouldClose()) {
    window.Clear();

    window.Update();
  }

  return 0;
}