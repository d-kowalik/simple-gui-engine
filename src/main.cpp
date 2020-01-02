#include <GLFW/glfw3.h>

#include <cstdio>

#include "Graphics/Window.hpp"

using namespace sge;

int main() {
    Window window{640, 480, "Simple GUI Engine"};
    while (!window.ShouldClose()) {
        window.Clear();

        window.Update();
    }

    return 0;
}