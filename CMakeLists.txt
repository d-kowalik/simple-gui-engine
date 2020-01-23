cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(SimpleGUIEngine)
set(CXX g++)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_STANDARD 17)
set(OpenGL_GL_PREFERENCE GLVND)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)

# Add .lib files
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} src/Util/FileUtil.hpp src/Graphics/Shader.cpp src/Graphics/Shader.hpp src/Graphics/ShaderProgram.cpp src/Graphics/ShaderProgram.hpp src/Input/Keycodes.hpp src/Input/Input.cpp src/Input/Input.hpp src/Camera.cpp src/Camera.hpp src/Graphics/Font.cpp src/Graphics/Font.hpp src/Graphics/FontRenderer.cpp src/Graphics/FontRenderer.hpp src/Util/Callback.hpp src/Graphics/RectangleRenderer.cpp src/Graphics/RectangleRenderer.hpp src/Application.cpp src/Application.hpp src/Graphics/ButtonRenderer.cpp src/Graphics/ButtonRenderer.hpp)

find_package(OpenGL REQUIRED)

# GLM
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

# GLFW
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

# freetype in lib folder, may be changed to somethin nicer like up there
find_library(FREETYPE freetype lib)

# GLAD
add_library(GLAD "thirdparty/glad.c")

# Put all libraries into a variable
set(LIBS glfw GL GLAD ${FREETYPE} dl)

# Define the include DIRs
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})
