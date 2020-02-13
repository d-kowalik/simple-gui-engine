# Simple GUI Engine


## Dependencies
- OpenGL `sudo apt-get install freeglut3 freeglut3-dev libglew1.5 libglew1.5-dev libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev on Ubuntu`
- FreeType2 `libfreetype6-dev on Ubuntu`
- GLAD (included)
- GLFW3 `libglfw3-dev on Ubuntu`
- GLM `libglm-dev on Ubuntu`

## How to compile
1. Create a build directory `mkdir build`
2. Enter build directory `cd build`
3.  Run `cmake ..`
4. Run `make`
5. **libsge.a** is the resulting library file

## Usage
Create a class deriving from `sge::Application` and use the `Run()` method.
Example:
```
#include <Application.hpp>
#include <cstdio>

class Program : public sge::Application {
  using sge::Application::Application;
  
  bool OnCreate() override {
    printf("Hello, world!");
    return true;
  }

  bool OnUpdate(float delta) override {
    return true;
  }
};

int main() {
  Program program{"Example", 1280, 720};
  program.Run();
}
```

## Methods available to override
- `bool OnCreate()` - called once when `Run()` is called
- `bool OnUpdate(float delta)` - called every frame
- `void OnKeyPressed(Key key)` - called when a key is pressed
- `void OnResize(int w, int h)` - called on resize
- virtual destructor

